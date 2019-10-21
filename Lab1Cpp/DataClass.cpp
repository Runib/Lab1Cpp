#include "DataClass.h"
#include <stdlib.h>
#include "DataClass.h"
#include <iostream>


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

void DataClass::print_data() {
    for(int i=0; i<rows; i++) {
        for(int j=0; j<columns; j++) {
            std::cout << "j: " <<  j << " , i:" << i << " , data:" << data[i][j] << std::endl;
        }
    }
}
