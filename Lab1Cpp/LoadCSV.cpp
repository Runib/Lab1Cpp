#include "LoadCSV.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include "DataClass.h"
#include <stdlib.h>


LoadCSV::LoadCSV()
{
}


LoadCSV::~LoadCSV()
{
}
void LoadCSV::myLoad(float *data, int rows, int columns)
{
	int i, j, k;
    string val;
    string line;


	ifstream file("mnist_train.csv");
	k=0;

	for (i = 0; i < rows; i++)
	{
		getline(file, line);
		if (!file.good())
			break;
		stringstream iss(line);

		for (j = 0; j < columns; j++)
		{

			getline(iss, val, ',');
			if (!iss.good())
				break;

            *(data+k) = atof(val.c_str());
            k++;
		}
	}
}

