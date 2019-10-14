#pragma once
class DataClass
{
public:
	DataClass();
	~DataClass();

	float **data;
	int rows = 6000;
	int columns = 729;

	float *label;
};

