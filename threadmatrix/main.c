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
int startRow = 0;
double cpu_time_used;
clock_t start, end;

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

void *workMethod(void * rowNum) {

	int i;
	for (i = startRow; i < startRow + size && (int)rowNum < size; i++) {
		multiplyRow(matrix1, matrix2, (int)rowNum, finalMatrix);
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
		pthread_create(&threads[num++], NULL, workMethod, startRow);
		startRow += theSize;
		totalRowsLeft -= theSize;
	}


	for (i = 0; i < numThreads; i++) {
		pthread_join(threads[i], NULL);
	}

	end = clock();
	printf("Threaded Matrix: \n");
	printMatrix(finalMatrix);
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Time to completion (serial): %2.10f\n", cpu_time_used);



}

int main() {

	loadMatrix(matrix1);
	loadMatrix(matrix2);

	printf("First Matrix\n");
	printMatrix(matrix1);

	printf("Second Matrix\n");
	printMatrix(matrix2);

	//serial

//	start = clock();
//	for (i = 0; i < size; i++) {
//		multiplyRow(matrix1, matrix2, i, finalMatrix);
//	}
//	end = clock();
//	printf("Serial final matrix: \n");
//	printMatrix(finalMatrix);
//
//	//Not a good measure
//	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
//	printf("Time to completion (serial): %2.10f\n", cpu_time_used);


	//Threaded version

	int i;
	for (i = 0; i < 5; i++) {
		runThreadedVersion(pow(2, i));
	}

	//Not a good measure


	return 0;
}
