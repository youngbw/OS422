/*
 * main.h
 *
 *  Created on: Apr 29, 2015
 *      Author: BrentYoung
 */

#ifndef MAIN_H_
#define MAIN_H_


static const int size = 10;
static const int num_cores = 4;


int gen_rand(int num);

void printMatrix(int matrix[][size]);

void multiplyRow(int matrix[][size], int matrix2[][size], int row, int finalMatrix[][size]);

void loadMatrix(int matrix[][size]);


typedef struct data {

	int matrix1[size][size];
	int matrix2[size][size];

	int finalMatrix[size][size];

	int row;

} data;

#endif /* MAIN_H_ */
