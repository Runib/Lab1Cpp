#include "Preprocessing.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <time.h>
#include <cmath>
#include <time.h>
#include <mpi.h>
#include "DataClass.h"


Preprocessing::Preprocessing()
{
}


Preprocessing::~Preprocessing()
{
}

void Preprocessing::Normalization(float *data, int rows, int columns)
{
	int min = 0, max = 0;
	int i = 0, j = 0;
	double startTime, endTime;
	int size, rank;
	int ROOT = 0;
	int tag = 2;

	MPI_Barrier(MPI_COMM_WORLD);
	startTime = MPI_Wtime();

	MPI_Status stat;
	MPI_Datatype dataTemp, dataColumns, dataTemp2, dataColumns2;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int countColumns = columns / size;
	//int sizeToSend = countColumns * columns;
	float *localData = (float*)malloc(countColumns * rows * sizeof(float));

	if (rank == 0)
	{
		MPI_Type_vector(rows, 1, columns, MPI_FLOAT, &dataTemp);
		MPI_Type_commit(&dataTemp);
		MPI_Type_create_resized(dataTemp, 0, 1*sizeof(float), &dataColumns);
		MPI_Type_commit(&dataColumns);
	}

	MPI_Type_vector(rows, 1, countColumns, MPI_FLOAT, &dataTemp2);
	MPI_Type_commit(&dataTemp2);
	MPI_Type_create_resized(dataTemp2, 0, 1*sizeof(float), &dataColumns2);
	MPI_Type_commit(&dataColumns2);

	MPI_Scatter(data, countColumns, dataColumns, localData, countColumns, dataColumns2,
		ROOT, MPI_COMM_WORLD);

	for (int i = 1; i < countColumns; i++)
	{
		min = 0;
		max = 0;
		for (int j = 0; j < rows; j++)
		{
			if (*(localData + i + (j*countColumns)) < min)
			{
				min = *(localData + (j*countColumns)+i);
			}

			if (*(localData + (j*countColumns)+i) > max)
			{
				max = *(localData + (j*countColumns)+i);
			}
		}

		float max_min_reciprocal = max - min;
		if (max_min_reciprocal == 0) {
			continue;
		}
		max_min_reciprocal = 1. / max_min_reciprocal;

		for (int j = 0; j < rows; j++)
		{
			//printf("Procek %d wykonuje\n ", rank);
			*(localData + (j*countColumns)+i) = (*(localData + (j*countColumns)+i) - min) * max_min_reciprocal;
		}
	}

	MPI_Gather(localData, countColumns, dataColumns2, data, countColumns, dataColumns, 
		ROOT, MPI_COMM_WORLD);

	printf("Procek %d Finished\n ", rank);

	// if (rank == 0)
	// {
	// 	for (int i = 0; i < columns; i++)
	// 	{
	// 		for (int j = 0; j < rows; j++)
	// 		{
	// 			printf("Data: %f, i: %d, j: %d\n", *(data + (j * columns) + i), i, j);
	// 		}
			
	// 	}
	// 	// DataClass datClass = DataClass();
	// 	// datClass.print_data();
		
	// }

	MPI_Barrier(MPI_COMM_WORLD);
	endTime = MPI_Wtime();

	if (rank == 0)
		printf("Czas obliczen Normalizacja: %f\n", endTime - startTime);

}

void Preprocessing::Standarization(float *data, int rows, int columns)
{
	int i = 0, j = 0;
    float var = 0, ave = 0, amo=0;
	float globalVar, globalEve, globalAmo;
	double startTime, endTime;
	int size, rank;
	int ROOT = 0;
	int tag = 2;

	MPI_Barrier(MPI_COMM_WORLD);
	startTime = MPI_Wtime();

	MPI_Status stat;
	MPI_Datatype dataTemp, dataColumns, dataTemp2, dataColumns2;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int countColumns = columns / size;
	//int sizeToSend = countColumns * columns;
	float *localData = (float*)malloc(countColumns * rows * sizeof(float));


	if (rank == 0)
	{
		MPI_Type_vector(rows, 1, columns, MPI_FLOAT, &dataTemp);
		MPI_Type_commit(&dataTemp);
		MPI_Type_create_resized(dataTemp, 0, 1*sizeof(float), &dataColumns);
		MPI_Type_commit(&dataColumns);
	}

	MPI_Type_vector(rows, 1, countColumns, MPI_FLOAT, &dataTemp2);
	MPI_Type_commit(&dataTemp2);
	MPI_Type_create_resized(dataTemp2, 0, 1*sizeof(float), &dataColumns2);
	MPI_Type_commit(&dataColumns2);

	MPI_Scatter(data, countColumns, dataColumns, localData, countColumns, dataColumns2,
		ROOT, MPI_COMM_WORLD);

	for (i = 1; i < countColumns - 1; i++)
	{
	    amo = 0;
        ave = 0;
        var = 0;
        for (int j = 0; j < rows; j++) {
            amo = amo + *(localData + (j*countColumns)+i);
        }
		//MPI_Reduce(&amo, &globalAmo, 1, MPI_FLOAT, MPI_SUM, ROOT, MPI_COMM_WORLD);
        ave  = amo / float(rows);

        for (int j = 0; j < rows; j++){
            float factor = *(localData + (j*countColumns)+i) - ave;
            var = var + (factor * factor);
        }
		//MPI_Reduce(&var, &globalVar, 1, MPI_FLOAT, MPI_SUM, ROOT, MPI_COMM_WORLD);

		if (var == 0) {
            for (j = 0; j < rows; j++) {
                *(localData + (j*countColumns)+i) = *(localData + (j*countColumns)+i) / 255.;
            }
            continue;
		}

        float sd_reciprocal = 1./sqrt(var);

		for (j = 0; j < rows; j++) {
            *(localData + (j*countColumns)+i) = (*(localData + (j*countColumns)+i) - ave) * sd_reciprocal;
		}

	}

	MPI_Gather(localData, countColumns, dataColumns2, data, countColumns, dataColumns, 
		ROOT, MPI_COMM_WORLD);

	printf("Procek %d Finished\n ", rank);

	// if (rank == 0)
	// {
	// 	for (int i = 0; i < countColumns; i++)
	// 	{
	// 		for (int j = 0; j < rows; j++)
	// 		{
	// 			printf("Data: %f, i: %d, j: %d\n", *(localData + (j * countColumns) + i), i, j);
	// 		}
			
	// 	}
	// 	// DataClass datClass = DataClass();
	// 	// datClass.print_data();
		
	// }
	MPI_Barrier(MPI_COMM_WORLD);
	endTime = MPI_Wtime();


	if (rank == 0)
		printf("Czas obliczen standaryzacja: %f\n", endTime - startTime);

}


