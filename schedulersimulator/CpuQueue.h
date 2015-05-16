/*
 * CpuQueue.h
 *
 *  Created on: May 14, 2015
 *      Author: BrentYoung
 */

#ifndef CPUQUEUE_H_
#define CPUQUEUE_H_

#define NR_PHASES 4

struct job;
typedef struct job {
	int job_id;
	int nr_phases;
	int current_phase;
	// Phase types: 0 = CPU phase; 1 = IO phase
	int phasetypes[NR_PHASES];
	int phasedurations[NR_PHASES];
	int isCompleted;
} job;


void passJob(job* theJob);
job* getJob(int type);
void* cpuWorkMethod(void* core);
void* ioWorkMethod(void* core);
void* finWorkMethod(void* core);



#endif /* CPUQUEUE_H_ */
