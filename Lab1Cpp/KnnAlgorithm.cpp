#include "KnnAlgorithm.h"
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <random>
#include <limits>
#include <cmath>
#include <omp.h>

using namespace std;

KnnAlgorithm::KnnAlgorithm()
{

}


KnnAlgorithm::~KnnAlgorithm()
{
}

void KnnAlgorithm::ShuffleData(float **data)
{
	random_device rd;
	mt19937 g(rd());
}

void KnnAlgorithm::SplitData(float **data, int percent)
{
	dataTrainRows = (6000 * percent)/100;
	dataTestRows = 6000 - dataTrainRows;

	int i;

	trainData = (float**)malloc(dataTrainRows * sizeof(float*));

	for (i = 0; i < dataTrainRows; i++)
		trainData[i] = (float*)malloc(columns * sizeof(float*));

	testData = (float**)malloc(dataTestRows * sizeof(float*));

	for (i = 0; i < dataTestRows; i++)
		testData[i] = (float*)malloc(columns * sizeof(float*));

	labelTrainData = (float*)malloc(dataTrainRows * sizeof(float*));
	labelTestData = (float*)malloc(dataTestRows * sizeof(float*));

	for (int i = 0; i < dataTrainRows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			trainData[i][j] = data[i][j];
		}
	}

	for (int i = 0; i <dataTestRows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			testData[i][j] = data[i + dataTrainRows][j];
		}
	}

	labelTrainData = trainData[0];
	labelTestData = testData[0];
}

float KnnAlgorithm::accuracy()
{
	float goodChoice = 0;
	int i = 0;
	int nr_threads = 1;

	double start, end;

	//start = omp_get_wtime();

	//#pragma omp parallel default(none) private(i, j, metric, amount, searchRow) shared(trainData, dataTrainRows, columns, nr_threads) num_threads(nr_threads)
	//#pragma omp for schedule(dynamic, nr_threads)
	for (i = 0; i < dataTrainRows; i++)
	{
		if (labelTrainData[(EuklidesMetric(i))] == labelTrainData[i])
			goodChoice = goodChoice + 1;
	}

	//end = omp_get_wtime();

	//printf("Czas obliczen: %f.\n", end - start);

	return goodChoice / float(dataTrainRows);
}

int KnnAlgorithm::EuklidesMetric(float row)
{
	float metric = std::numeric_limits<float>::max();
	float searchRow = 0;
	float amount;

	int myRow = int(row);

	for (int i = 0; i < dataTrainRows; i++)
	{
		amount = 0;

		if (i == myRow)
			continue;

		for (int j = 1; j < columns; j++)
		{
			amount = amount + pow(trainData[i][j] - trainData[myRow][j], 2);
		}

        float amount_sqrt = sqrt(amount);

		if (metric > amount_sqrt)
		{
			metric = amount_sqrt;
			searchRow = i;
		}

	}

	return searchRow;
}
