/*
 * Queue.h
 * Created on: May 14, 2015
 * Author: Brent Young
 * Author: Jonathan Stone
 */


#ifndef QUEUE_H_
#define QUEUE_H_

#include <pthread.h>
#include "CpuQueue.h"

#define IS_ENQUEUE 1
#define IS_DEQUEUE 0

struct Node;
typedef struct Node {

	job* data;
	struct Node* next;


} Node;

typedef struct Queue {

	pthread_mutex_t queueMutex;
	struct Node* front;
	struct Node* back;
	int size;

} Queue_t;


Queue_t* getNewQueue();

job* performQueueOperation(Queue_t* theQueue, int isEnqueue, job* theJob);

job* deQueue(Queue_t* theQueue);

job* enQueue(Queue_t* theQueue, job* theJob);

int isEmpty(Queue_t* theQueue);

int size(Queue_t* theQueue);



#endif /* QUEUE_H_ */
