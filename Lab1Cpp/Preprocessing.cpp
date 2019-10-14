#include "Preprocessing.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>



Preprocessing::Preprocessing()
{
}


Preprocessing::~Preprocessing()
{
}


void Preprocessing::Normalization(float **data, int rows, int columns)
{
	float *min, *max;

	min = (float*)malloc(columns * sizeof(float));
	max = (float*)malloc(columns * sizeof(float));


	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (data[i][j] < min[j])
			{
				min[j] = data[i][j];
			}

			if (data[i][j] > max[j])
			{
				max[j] = data[i][j];
			}
		}
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			data[i][j] = (data[i][j] - min[j]) / (max[j] - min[j]);
		}
	}
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
		for (j = 0; j < columns; j++)
		{
			amount[j] = amount[j] + data[i][j];
		}
	}

	for (int j = 0; j < columns; j++)
	{
		average[j] = amount[j] / 60000;
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			variance[j] = variance[j] + pow(data[i][j] - average[j], 2);
		}
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			data[i][j] = (data[i][j] - average[j]) / sqrt(variance[j]);
		}
	}
}


