#include "DataClass.h"
#include <stdlib.h>
#include "DataClass.h"
#include <iostream>


DataClass::DataClass()
{
	int i;

//	data = (float**)malloc(rows * sizeof(float*));

//	for (i = 0; i < rows; i++)
//		data[i] = (float*)malloc(columns * sizeof(float*));

//	label = (float*)malloc(rows * sizeof(float));

	data = (float*) malloc (rows * columns * sizeof(float));
}


DataClass::~DataClass()
{
}

void DataClass::print_data() {
    int elements_count = columns * rows;
    for(int i=0; i<elements_count; i++) {
            std::cout << " , i:" << i << " , data:" << *(data+i) << std::endl;
    }
}

void DataClass::print_data_not_value() {
    int elements_count = columns * rows;
    for(int i=0; i<elements_count; i++) {
            if (*(data+i) > 1 || *(data+i) < 0)
                printf("Blad w normalizacji: %d, %f\n", i, *(data+i));
    }
}
