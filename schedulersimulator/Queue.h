/*
 * Queue.h
 * Created on: May 14, 2015
 * Author: BrentYoung
 */


#ifndef QUEUE_H_
#define QUEUE_H_

#include <pthread.h>
#include "CpuQueue.h"

static const int IS_ENQUEUE = 1;
static const int IS_DEQUEUE = 0;

typedef struct Node {

	job* data;
	struct Node_t* next;


} Node_t;

typedef struct Queue {

	pthread_mutex_t queueMutex;
	Node_t* front;
	Node_t* back;
	int size;

} Queue_t;


Queue_t* getNewQueue();

job* performQueueOperation(Queue_t* theQueue, int isEnqueue, job* theJob);

job* deQueue(Queue_t* theQueue);

job* enQueue(Queue_t* theQueue, job* theJob);

int isEmpty(Queue_t* theQueue);

int size(Queue_t* theQueue);



#endif /* QUEUE_H_ */
