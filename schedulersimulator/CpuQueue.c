/*
 * CpuQueue.c
 *
 *  Created on: May 14, 2015
 *      Author: BrentYoung
 */
#include <stdio.h>
#include <stdlib.h>
#include <Queue.h>
#include <pthread.h>

struct job *cpu[8];
struct job *io[4];
struct job *fin[4];

CPU_Queue;
IO_Queue;
Fin_Queue;

void cpuWorkMethod(void* core_num) {
	job* theJob = cpu[core_num];
	if (theJob->phasedurations[theJob->current_phase] == 0) {
		passJob(theJob);
		getJob();
	}
}

void passJob(job* theJob) {
	theJob->current_phase += 1;
	if(theJob->nr_phases - theJob->current_phase <= 1) {
		performQueueOperation(Fin_Queue, IS_ENQUEUE, theJob);
	} else {
		if(theJob->phasetypes[theJob->current_phase]) {
			performQueueOperation(IO_Queue, IS_ENQUEUE, theJob);
		} else {
			performQueueOperation(CPU_Queue, IS_ENQUEUE, theJob);
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

void ioWorkMethod(void *core_num) {
	job* theJob = io[core_num];
	if (theJob->phasedurations[theJob->current_phase] == 0) {
		passJob(theJob);
		getJob();
	}
}

void finWorkMethod() {
	getJob();
	// delete job
}

int main() {
	int i;

	CPU_Queue = getNewQueue();
	IO_Queue = getNewQueue();
	Fin_Queue = getNewQueue();

	pthread_t threads[16];
	int finished = 0;
	for(i = 0; i < 8;) {
		pthread_create(&threads[i], NULL, cpuWorkMethod, NULL);
		if (i < 4) {
			pthread_create(&threads[i+8], NULL, ioWorkMethod, NULL);
			pthread_create(&threads[i+12], NULL, finWorkMethod, NULL);
		}
	}

	while (finished == 0) {
		for(i = 0; i < 8;) {

			if (i < 4) {
			}
		}


		if (isEmpty(CPU_Queue) &&
			isEmpty(IO_Queue) &&
			isEmpty(Fin_Queue)) {
			finished = 1;
		}
	}

	return 0;
}
