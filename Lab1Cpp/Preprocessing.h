#pragma once
class Preprocessing
{
public:
	Preprocessing();
	~Preprocessing();

	void Normalization(float **data, int rows, int columns);

	void Standarization(float **data, int rows, int columns);
};

