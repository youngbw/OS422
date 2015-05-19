/*
 * CpuQueue.c
 *
 *  Created on: May 14, 2015
 *      Author: Brent Young
 *	Author: Jonathan Stone
 *	Author: Huy Ngo
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
FILE *fp;

clock_t start_time;
clock_t end_time;

int finished;
//job id being incremented
int jobId;
//Target amount of jobs
int amountOfJobs = 12;
//Last time a job was created
double lastTime;
//Amount of seconds to inject jobs
int targetSeconds = 2;
//Number of total jobs created
int numJobsCreated = 0;
//Number of jobs successfully completed and memory freed
int numJobsFreed = 0;

void* cpuWorkMethod(void* core) {
	int core_num = (int)(long)core;
	while(finished == 0) {
		job* theJob = cpu[core_num];

		if (theJob == NULL) {
			theJob = getJob(0);
			cpu[core_num] = theJob;
			if(theJob != NULL){
				fprintf(fp, "Job ID: %d is starting on CPU %d \n", cpu[core_num]->job_id, core_num);
			}
		}
		if (theJob != NULL && theJob->phasedurations[theJob->current_phase] == 0) {
			passJob(theJob);
			fprintf(fp, "Job ID: %d moved off of CPU %d \n", theJob->job_id, core_num);
			theJob = getJob(0);
			cpu[core_num] = theJob;
			if(theJob != NULL){
				fprintf(fp, "Job ID: %d started on CPU %d \n", cpu[core_num]->job_id, core_num);
			}
		}
		sleep(1);
	}
	return 0;
}

void passJob(job* theJob) {
	theJob->current_phase += 1;
	if(theJob->nr_phases - theJob->current_phase <= 1) {
		performQueueOperation(Fin_Queue, IS_ENQUEUE, theJob);
		fprintf(fp, "Job ID: %d moved onto finished queue\n", theJob->job_id);
	} else {
		if(theJob->phasetypes[theJob->current_phase]) {
			performQueueOperation(IO_Queue, IS_ENQUEUE, theJob);
			fprintf(fp, "Job ID: %d moved onto I/O queue\n", theJob->job_id);
		} else {
			performQueueOperation(CPU_Queue, IS_ENQUEUE, theJob);
			fprintf(fp, "Job ID: %d moved onto CPU/Ready queue\n", theJob->job_id);
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
	int core_num = (int)(long)core;
	while(finished == 0) {
		job* theJob = io[core_num];
		if (theJob == NULL) {
			theJob = getJob(1);
			io[core_num] = theJob;
		}
		if (theJob != NULL && theJob->phasedurations[theJob->current_phase] == 0) {
			passJob(theJob);
			fprintf(fp, "Job ID: %d moved off of IO stream\n", theJob->job_id);
			theJob = getJob(1);
			io[core_num] = theJob;
			if (theJob != NULL) {
				fprintf(fp, "Job ID: %d started on I/O stream\n", io[core_num]->job_id);
			}
		}
		sleep(1);
	}
	return 0;
}

void* finWorkMethod(void* core) {
	int core_num = (int)(long)core;
	while(finished == 0) {
		job* theJob = createJob();
		if (theJob == NULL) {
			theJob = getJob(2);
			//fin[core_num] = theJob;
			if (theJob != NULL) {
				fprintf(fp, "Freeing memory for Job ID: %d\n", theJob->job_id);
				free(theJob);
				numJobsFreed++;
				//fin[core_num] = getJob(2);
			}
		} else {
			performQueueOperation(CPU_Queue, IS_ENQUEUE, theJob);
		}
		sleep(1);
	}

	return 0;
}

job* createJob() {
	double targetTime = lastTime + targetSeconds * CLOCKS_PER_SEC;
	double currentTime = clock();
	if ( currentTime >= targetTime || numJobsCreated < amountOfJobs) {
		numJobsCreated++;
		job* theJob = malloc(sizeof(job));
		theJob->current_phase = 0;
		theJob->job_id = jobId;
		theJob->nr_phases = 4;
		theJob->phasedurations[0] = 5;
		theJob->phasedurations[1] = 5;
		theJob->phasedurations[2] = 5;
		theJob->phasedurations[3] = 5;
		theJob->phasetypes[0] = 0;
		theJob->phasetypes[1] = 1;
		theJob->phasetypes[2] = 0;
		theJob->phasetypes[3] = 1;

    		jobId++;
    		lastTime = currentTime;

    		fprintf(fp, "Job ID: %i created!\n", theJob->job_id);
    		return theJob;
	}
	return NULL;
}

int main(int argc, char* argv[]) {
	long i;
	fp = fopen("output.txt", "w");

    lastTime = clock() + targetSeconds * CLOCKS_PER_SEC;
//	start_time = gmtime(start_time);

	CPU_Queue = getNewQueue();
	IO_Queue = getNewQueue();
	Fin_Queue = getNewQueue();

	//Creating jobs
	for(i = 0; i < amountOfJobs; i++) {
        job* theJob = createJob();
        performQueueOperation(CPU_Queue, IS_ENQUEUE, theJob);
	}


	pthread_t threads[16];
	finished = 0;
	for(i = 0; i < 8; i++) {
		//fprintf("Creating thread #%ld!\n", i);
		pthread_create(&threads[i], NULL, cpuWorkMethod, (void *)i);
		if (i < 4) {
			//fprintf("Creating thread #%ld!\n", i+8);
			pthread_create(&threads[i+8], NULL, ioWorkMethod, (void *)i);
			//fprintf("Creating thread #%ld!\n", i+12);
			pthread_create(&threads[i+12], NULL, finWorkMethod, (void *)i);
		}

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
			isEmpty(Fin_Queue) && (numJobsCreated == numJobsFreed)) {
			finished = 1;
		}
	}


	for(i = 0; i < 16; i++) {
		pthread_join(threads[i], NULL);
	}
	fprintf(fp, "Work complete\n");
	return 0;
}
