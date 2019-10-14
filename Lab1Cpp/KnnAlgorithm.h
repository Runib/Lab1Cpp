#pragma once
class KnnAlgorithm
{
public:
	KnnAlgorithm();
	~KnnAlgorithm();

	int columns = 729;
	int k = 1;

	int dataTrainRows = 4500;
	int dataTestRows = 1500;

	float **trainData;
	float **testData;

	float *labelTrainData;
	float *labelTestData;
	float accuracy();

	void SplitData(float **data, int percent);
private:
	void ShuffleData(float **data);


	float *EuklidesMetric(float row);
	int classify(float *metric);
};

