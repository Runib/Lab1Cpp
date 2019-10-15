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

float **Preprocessing::Normalization(float **data, int rows, int columns)
{
	double start, end;
	int min = 0, max = 0;
	int i = 0, j = 0;
	int nr_threads = 1;

	start = omp_get_wtime();

	#pragma omp parallel default(none) private(i, j, min, max) shared(data, rows, columns, nr_threads) num_threads(nr_threads)
	#pragma omp for schedule(dynamic, nr_threads)
	for (int i = 1; i < columns-1; i++)
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

	return data;
}

float **Preprocessing::Standarization(float **data, int rows, int columns)
{
	double start, end;
	float amo = 0, var = 0, ave = 0;
	int nr_threads = 1;
	int i = 0, j = 0;

	start = omp_get_wtime();

	#pragma omp parallel default(none) private(i, j, amo, var, ave) shared(data, rows, columns, nr_threads) num_threads(nr_threads)
	#pragma omp for schedule(dynamic, nr_threads)
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
			data[j][i] = (data[j][i] - ave) / sqrt(var);
		}

	}

	end = omp_get_wtime();

	printf("Czas obliczen standaryzacja: %f.\n", end - start);

	return data;
}


