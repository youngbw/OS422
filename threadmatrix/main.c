/*
 * main.c
 *
 *  Created on: Apr 29, 2015
 *      Author: BrentYoung
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

static const int size = 10;

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


	return 0;
}
