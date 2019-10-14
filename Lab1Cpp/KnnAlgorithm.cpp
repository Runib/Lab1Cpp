#include "KnnAlgorithm.h"
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <random>

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
	int goodChoice = 0;

	for (int i = 0; i < dataTrainRows; i++)
	{
		if (labelTrainData[classify(EuklidesMetric(i))] == labelTrainData[i])
			goodChoice++;
	}

	return goodChoice / dataTrainRows;
}

float *KnnAlgorithm::EuklidesMetric(float row)
{
	float *metric = (float*)malloc(dataTrainRows * sizeof(float));
	float amount;

	int myRow = int(row);

	for (int i = 0; i < dataTrainRows; i++)
	{
		amount = 0;

		if (i == myRow)
			continue;

		for (int j = 0; j < columns; j++)
		{
			amount = amount + pow(trainData[i][j] - trainData[myRow][j], 2);
		}
		metric[i] = sqrt(amount);
	}

	return metric;
}

int KnnAlgorithm::classify(float *metric)
{
	float min = metric[0];
	int i = 0;

	for (int i = 1; i < dataTrainRows; i++)
	{
		if (metric[i] < min)
			return i;
	}

	return i;
}
