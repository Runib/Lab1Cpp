#include "LoadCSV.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include "DataClass.h"



LoadCSV::LoadCSV()
{
}


LoadCSV::~LoadCSV()
{
}

void LoadCSV::myLoad(float **data, int rows, int columns)
{
	int i, j, k;


	ifstream file("mnist_train.csv");

	for (i = 0; i < rows; ++i)
	{
		string line;
		getline(file, line);
		if (!file.good())
			break;
		stringstream iss(line);

		for (j = 0; j < columns; ++j)
		{
			string val;
			getline(iss, val, ',');
			if (!iss.good())
				break;

			stringstream convertor(val);
			convertor >> data[i][j];
		}
	}
}

