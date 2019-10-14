#include "Preprocessing.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <omp.h>
#include <time.h>



Preprocessing::Preprocessing()
{
}


Preprocessing::~Preprocessing()
{
}

float **Preprocessing::Normalization(float **data, int rows, int columns)
{
	time_t begin_t, end_t;
	int min = 0, max = 0;
	int i = 0, j = 0;

	begin_t = time(NULL);

	#pragma omp parallel default(none) private(i, j, min, max) shared(data, rows, columns)
	#pragma omp for schedule(static, 4)
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

	end_t = time(NULL);

	printf("Czas obliczen: %f.\n", difftime(end_t, begin_t));

	return data;
}

void Preprocessing::Standarization(float **data, int rows, int columns)
{
	float *amount, *variance;
	int i, j;

	float *average = (float*)malloc(columns * sizeof(float));
	amount = (float*)malloc(columns * sizeof(float));
	variance = (float*)malloc(columns * sizeof(float));

	for (i = 0; i < rows; i++)
	{
		for (j = 1; j < columns; j++)
		{
			amount[j] = amount[j] + data[i][j];
		}
	}

	for (int j = 1; j < columns; j++)
	{
		average[j] = amount[j] / 60000;
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 1; j < columns; j++)
		{
			variance[j] = variance[j] + pow(data[i][j] - average[j], 2);
		}
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 1; j < columns; j++)
		{
			data[i][j] = (data[i][j] - average[j]) / sqrt(variance[j]);
		}
	}
}


