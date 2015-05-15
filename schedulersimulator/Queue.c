#include <stdio.h>
#include <stdlib.h>


Queue* getNewQueue() {

	Queue* newQueue = malloc(sizeof(Queue));
	newQueue->size = 0
	newQueue->front = NULL;
	newQueue->back = NULL;
	return newQueue;

}

job* performQueueOperation(Queue* theQueue, int isEnqueue, job* theJob) {

	job* aJob;

	if (isEnqueue == IS_ENQUEUE) {
		aJob = enQueue(theQueue, theJob);
	} else {
		aJob = deQueue(theQueue);
	}

	return aJob;

}



job* enQueue(Queue* theQueue, job* theJob) {

	Node* theNode = malloc(sizeof(Node));
	theNode->data = theJob;
	theQueue->back->next = theNode;
	theNode->next = NULL;
	theQueue->back = theNode;
	theQueue->size++;

	return theQueue->back;


}


job* deQueue(Queue* theQueue) {
	job* theJob = NULL;
	if (!isEmpty(theQueue)) {
		Node* theNode = theQueue->front;
		theQueue->front = theNode->next;
		theJob = theNode->data;
		theQueue->size--;
		free(theNode);
	}
	return theJob;

}

int isEmpty(Queue* theQueue) {

	return theQueue->size == 0;

}



