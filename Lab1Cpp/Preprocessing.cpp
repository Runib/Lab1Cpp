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
	#pragma omp for schedule(dynamic, numberOfThreads)
	for (int i = 1; i < columns; i++)
	{
		min = 0;
		max = 0;
		for (int j = 0; j < rows; j++)
		{
			if (data[j][i] < min)
			{
				min = data[j][i];
			}
			if (data[j][i] > max)
			{
				max = data[j][i];
			}
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
	int i = 0, j = 0;
	float amo = 0, var = 0, ave = 0;

	start = omp_get_wtime();

	#pragma omp parallel default(none) private(i, j, min, max) shared(data, rows, columns, numberOfThreads) num_threads(numberOfThreads)
	#pragma omp for schedule(dynamic, numberOfThreads)
	for (i = 1; i < columns - 1; i++)
	{
		ave = 0;
		var = 0;
		amo = 0;
		for (j = 0; j < rows; j++)
		{
			amo = amo + data[j][i];
		}
		ave = amo / float(rows);
		for (j = 0; j < rows; j++)
		{
			var = var + pow(data[j][i] - ave, 2);
		}
		var = var / float(rows);
		
		for (j = 0; j < rows; ++j)
		{
			if (var != 0)
				data[j][i] = (data[j][i] - ave) / sqrt(var);
			else
				data[j][i] = (data[j][i] / float(255));
		}

	}

	end = omp_get_wtime();

	printf("Czas obliczen standaryzacja: %f.\n", end - start);
}
