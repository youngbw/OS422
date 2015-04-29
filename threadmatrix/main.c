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


int gen_rand(int num) {
	if (num == 0) {
		srand(10);
		return rand() % 10 + 1;
	} else {
		return num * 7 / 3 % 10 + 1;
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

void multiplyRow(int matrix[][size], int matrix2[][size], int row, int finalMatrix[][size]) {
	int col = 0;
	int num = 0;
	int local = 0;
	while (col < size) {
		num = 0;
		local = 0;
		while (local < size) {
			num += matrix[row][local] * matrix2[local][col];
			local++;
		}
		finalMatrix[row][col] = num;
		col++;
	}
}

void *workMethod(void * data1) {

	struct data* theData = data1;
//	int row = ((struct data*)data1)->row;
//	int* matrix1[][size] = (&(struct data *)data1)->matrix1;
//	int* matrix2[][size] = ((struct data *)data1)->matrix2;
//	int* finalMatrix[][size] = ((struct data *)data1)->finalMatrix;

	multiplyRow(theData->matrix1, theData->matrix2, theData->row, theData->finalMatrix);

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

int createThreads(int matrix[][size], int matrix2[][size], int finalMatrix[][size]) {
	pthread_t threads[num_cores];
	int row;
	int numThreadsCreated = 0;

	/*the following 5 lines demonstrates how to create 1 thread to calculate C[0][0], you
			will need to create a loop for all of C's cells*/
	int i;
	for (i = 0; i < num_cores; i++) {
		struct data *data1 = (struct data *)malloc(sizeof(struct data));
		data1->matrix1 = matrix;
		data1->matrix2 = matrix2;
		data1->finalMatrix = finalMatrix;
		data1->row = row++; //assign the row of C for thread to calculate
		pthread_create(&threads[0], NULL, workMethod, data1);
		numThreadsCreated++;
	}

	int j;
	for(j=0; j < numThreadsCreated; j++) {
				pthread_join( threads[j], NULL);
	}

	return numThreadsCreated;
}


int main() {

	clock_t start, end;
	double cpu_time_used;





	int matrix1[size][size];
	int matrix2[size][size];

	int finalMatrix[size][size]; //for serial
	int finalMatrix2[size][size]; //for threaded

	loadMatrix(matrix1);
	loadMatrix(matrix2);

	printf("First Matrix\n");
	printMatrix(matrix1);

	printf("Second Matrix\n");
	printMatrix(matrix2);

	//serial
	int i;
	start = clock();
	for (i = 0; i < size; i++) {
		multiplyRow(matrix1, matrix2, i, finalMatrix);
	}
	end = clock();
	printf("Serial final matrix: \n");
	printMatrix(finalMatrix);

	//Not a good measure
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Time to completion (serial): %2.10f\n", cpu_time_used);


	//Threaded version
	int rowsCompleted = 0;
	start = clock();
	while (rowsCompleted < size) {

		rowsCompleted += createThreads(matrix1, matrix2, finalMatrix2);

	}
	end = clock();
	printf("Threaded Matrix: \n");
	printMatrix(finalMatrix2);

	//Not a good measure
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Time to completion (serial): %2.10f\n", cpu_time_used);

	return 0;
}
