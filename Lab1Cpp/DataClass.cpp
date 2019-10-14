#include "DataClass.h"
#include <stdlib.h>



DataClass::DataClass()
{
	int i;

	data = (float**)malloc(rows * sizeof(float*));

	for (i = 0; i < rows; i++)
		data[i] = (float*)malloc(columns * sizeof(float*));

	label = (float*)malloc(rows * sizeof(float));
}


DataClass::~DataClass()
{
}
