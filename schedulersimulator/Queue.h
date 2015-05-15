/*
 * Queue.h
 * Created on: May 14, 2015
 * Author: BrentYoung
 */


#ifndef QUEUE_H_
#define QUEUE_H_



static const int IS_ENQUEUE = 1;
static const int IS_DEQUEUE = 0;


typedef struct Queue {

	Node* front;
	Node* back;
	int size;

} Queue;



typedef struct Node {

	job* data;
	Node* next;


} Node;

Queue* getNewQueue();

Node* performQueueOperation(int isEnqueue);

int isEmpty();


#endif /* QUEUE_H_ */