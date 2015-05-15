/*
 * CpuQueue.c
 *
 *  Created on: May 14, 2015
 *      Author: BrentYoung
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "CpuQueue.h"
#include "Queue.h"



job *cpu[8];
job *io[4];
job *fin[4];

Queue_t* CPU_Queue;
Queue_t* IO_Queue;
Queue_t* Fin_Queue;

clock_t start_time;
clock_t end_time;

void* cpuWorkMethod(void* core) {
	int core_num = (long)core;
	job* theJob = cpu[core_num];
	if (theJob != NULL && theJob->phasedurations[theJob->current_phase] == 0) {
		passJob(theJob);
		printf("Job ID: %d moved off of CPU", theJob->job_id);
		cpu[core_num] = getJob(0);
		printf("Job ID: %d started on CPU", cpu[core_num]->job_id);
	}
	return 0;
}

void passJob(job* theJob) {
	theJob->current_phase += 1;
	if(theJob->nr_phases - theJob->current_phase <= 1) {
		performQueueOperation(Fin_Queue, IS_ENQUEUE, theJob);
		printf("Job ID: %d moved onto finished queue", theJob->job_id);
	} else {
		if(theJob->phasetypes[theJob->current_phase]) {
			performQueueOperation(IO_Queue, IS_ENQUEUE, theJob);
			printf("Job ID: %d moved onto I/O queue", theJob->job_id);
		} else {
			performQueueOperation(CPU_Queue, IS_ENQUEUE, theJob);
			printf("Job ID: %d moved onto CPU/Ready queue", theJob->job_id);
		}
	}
}

job* getJob(int type) {
	if(type == 0) {
		return performQueueOperation(CPU_Queue, IS_DEQUEUE, NULL);
	} else if (type == 1) {
		return performQueueOperation(IO_Queue, IS_DEQUEUE, NULL);
	} else {
		return performQueueOperation(Fin_Queue, IS_DEQUEUE, NULL);
	}
}

void* ioWorkMethod(void *core) {
	int core_num = (long)core;
	job* theJob = io[core_num];
	if (theJob != NULL && theJob->phasedurations[theJob->current_phase] == 0) {
		passJob(theJob);
		printf("Job ID: %d moved off of IO stream", theJob->job_id);
		io[core_num] = getJob(1);
		printf("Job ID: %d started on I/O stream", io[core_num]->job_id);
	}
	return 0;
}

void* finWorkMethod(void* num) {
	job* theJob = getJob(2);
	if (theJob != NULL) {
		printf("Freeing memory for Job ID: %d", theJob->job_id);
		free(theJob);
	}
	return 0;
}

int main() {
	long i;
//	start_time = gmtime(start_time);
	CPU_Queue = getNewQueue();
	IO_Queue = getNewQueue();
	Fin_Queue = getNewQueue();

	pthread_t threads[16];
	int finished = 0;
	for(i = 0; i < 8; i++) {
		pthread_create(&threads[i], NULL, cpuWorkMethod, (void *)i);
		if (i < 4) {
			pthread_create(&threads[i+8], NULL, ioWorkMethod, (void *)i);
			pthread_create(&threads[i+12], NULL, finWorkMethod, (void *)i);
		}
	}

	for(i = 0; i < 20; i++) {
			job* theJob = malloc(sizeof(job));
			theJob->current_phase = 0;
			theJob->job_id = i;
			theJob->nr_phases = 4;
			theJob->phasedurations[0] = 5;
			theJob->phasedurations[1] = 5;
			theJob->phasedurations[2] = 5;
			theJob->phasedurations[3] = 5;
			theJob->phasetypes[0] = 1;
			theJob->phasetypes[0] = 0;
			theJob->phasetypes[0] = 1;
			theJob->phasetypes[0] = 0;
			performQueueOperation(CPU_Queue, IS_ENQUEUE, theJob);
	}

	while (finished == 0) {
		job* theJob;
		for(i = 0; i < 8; i++) {
			theJob = cpu[i];
			if (theJob != NULL && theJob->phasedurations[theJob->current_phase] != 0) {
				theJob->phasedurations[theJob->current_phase]--;
			}


			if (i < 4) {
				theJob = io[i];
				if (theJob != NULL && theJob->phasedurations[theJob->current_phase] != 0) {
								theJob->phasedurations[theJob->current_phase]--;
				}
				theJob = fin[i];
				if (theJob != NULL && theJob->phasedurations[theJob->current_phase] != 0) {
								theJob->phasedurations[theJob->current_phase]--;
				}
			}
		}


		if (isEmpty(CPU_Queue) &&
			isEmpty(IO_Queue) &&
			isEmpty(Fin_Queue)) {
			finished = 1;
		}
	}


	for(i = 0; i < 16; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}
