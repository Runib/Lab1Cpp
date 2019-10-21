#include "Preprocessing.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <omp.h>
#include <time.h>
#include <cmath>




Preprocessing::Preprocessing()
{
}


Preprocessing::~Preprocessing()
{
}

void Preprocessing::Normalization(float **data, int rows, int columns, int numberOfThreads)
{
	double start, end;
	float min = 0, max = 0;
	int i = 0, j = 0;

	start = omp_get_wtime();

	#pragma omp parallel default(none) private(i, j, min, max) shared(data, rows, columns, numberOfThreads) num_threads(numberOfThreads)
	for (int i = 1; i < columns; i++)
	{
		min = 0;
		max = 0;
		for (int j = 0; j < rows; j++)
		{
			min = findMin(data[j][i], min);
			max = findMax(data[j][i], max);
		}

		for (int j = 0; j < rows; j++)
		{
			if (min != 0 || max != 0)
				data[j][i] = (data[j][i] - min) / (max - min);
		}

	}

	end = omp_get_wtime();

	printf("Czas obliczen normalizacja: %f.\n", end - start);
}

void Preprocessing::Standarization(float **data, int rows, int columns, int numberOfThreads)
{
	double start, end;
	float variance = 0, average = 0;
	int i = 0, j = 0;

	start = omp_get_wtime();

	#pragma omp parallel default(none) private(i, j, variance, average) shared(data, rows, columns, numberOfThreads) num_threads(numberOfThreads)
	#pragma omp for schedule(dynamic, nr_threads)
	for (i = 1; i < columns; i++, average = 0, variance = 0)
	{
		average = getAverage(data, rows, i);
		variance = getVariance(data, rows, i, average);
		
		for (j = 0; j < rows; ++j)
		{
			if (variance != 0)
				data[j][i] = (data[j][i] - average) / sqrt(variance);
			else
				data[j][i] = (data[j][i] / float(255));
		}

	}

	end = omp_get_wtime();

	printf("Czas obliczen standaryzacja: %f.\n", end - start);
}

float Preprocessing::findMin(float data, float currentMin)
{
	if (data < currentMin)
		return data;
	else return currentMin;
}

float Preprocessing::findMax(float data, float currentMax)
{
	if (data > currentMax)
		return data;
	else return currentMax;
}

float Preprocessing::getAverage(float ** data, int rows, int currentlyColumns)
{
	float amount = 0;

	for (int j = 0; j < rows; j++)
	{
		amount = amount + data[j][currentlyColumns];
	}
	return amount / float(rows);
}

float Preprocessing::getVariance(float ** data, int rows, int currentlyColumns, float average)
{
	float variance = 0;
	for (int j = 0; j < rows; j++)
	{
		variance = variance + pow(data[j][currentlyColumns] - average, 2);
	}
	return variance / float(rows);
}



