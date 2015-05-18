/*
 * main.c
 *
 *  Created on: Apr 29, 2015
 *      Author: BrentYoung
 */

#include "main.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int matrix1[size][size];
int matrix2[size][size];
int finalMatrix[size][size]; //for serial
int totalRowsLeft = size;
int theSize = size;
long startRow = 0;
double cpu_time_used;
clock_t start;

int gen_rand(int num) {
	if (num == 0) {
		srand(10);
		return rand() % 10 + 1;
	} else {
		return rand() % 10 + 1;
	}


}

void printMatrix(int matrix[][size]) {
	int row = 0;
	int col = 0;
	while (row < size) {
		while (col < size) {
			printf("%d ", matrix[row][col++]);
		}
		row++;
		col = 0;
		printf("\n");
	}
	printf("\n");
}

void multiplyRow(int row) {

	int col = 0;
	int num = 0;
	int local = 0;
	while (col < size) {
		num = 0;
		local = 0;
		while (local < size) {
			num += matrix1[row][local] * matrix2[local][col];
			local++;
		}
		finalMatrix[row][col] = num;
		col++;
	}

}

void *workMethod(void * rowNum) {
	long row = (long)rowNum;

	int i;
	for (i = row; i < row + theSize && row < size; i++) {
		multiplyRow(i);
	}
	return NULL;
}

void loadMatrix(int matrix[][size]) {
	int col = 0;
	int row = 0;
	int prev = gen_rand(0);
	while (row < size) {
		col = 0;
		while (col < size) {
			matrix[row][col] = prev;
			prev = gen_rand(prev);
			col++;
		}
		row++;
	}
}


void runThreadedVersion(int numThreads) {
	int i;
	//reset for next run
	totalRowsLeft = size;
	startRow = 0;
	printf("Number of threads for this run: %d", numThreads);
	pthread_t threads[numThreads];
	theSize = size / numThreads;

	start = clock();
	int num = 0;
	while (totalRowsLeft > 0) {
		pthread_t thread;
		pthread_create(&threads[num++], NULL, workMethod, (void *)startRow);
		startRow += theSize;
		totalRowsLeft -= theSize;
	}


	for (i = 0; i < numThreads; i++) {
		pthread_join(threads[i], NULL);
	}

	start = clock() - start;
	printf("Threaded Matrix: \n");
	printMatrix(finalMatrix);
	cpu_time_used = ((double)start) / CLOCKS_PER_SEC;
	printf("Time to completion: %2.10f\n", cpu_time_used);

	pthread_exit(NULL);

}

int main(int argc, char* argv[]) {
	int threads = atoi(argv[1]);

	if (threads > size) {
		printf("Threads greater than the size of the matrix\n");
		return 0;
	}

	loadMatrix(matrix1);
	loadMatrix(matrix2);

	printf("First Matrix\n");
	printMatrix(matrix1);

	printf("Second Matrix\n");
	printMatrix(matrix2);


	runThreadedVersion(threads);


	return 0;
}
