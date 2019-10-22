#pragma once
class Preprocessing
{
public:
	Preprocessing();
	~Preprocessing();

    void Normalization(float *data, int rows, int columns, int numberOfThreads);

	void Standarization(float *data, int rows, int columns, int numberOfThreads);
private:
	float findMin(float data, float currentMin);

	float findMax(float data, float currentMax);
};

