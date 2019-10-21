#pragma once
#include "DataClass.h"
class KnnAlgorithm
{
public:
	KnnAlgorithm();
	~KnnAlgorithm();

	void fit(DataClass data, int percent);
	float predict();
	float **trainData;
	float **testData;
	int dataTrainRows;
	int dataTestRows;
	int dataColumns;

};