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
#include <iostream>


Preprocessing::Preprocessing()
{
}


Preprocessing::~Preprocessing()
{
}

void Preprocessing::Normalization(float *data, int rows, int columns)
{
	int min = 0, max = 0;
	int globalMin, globalMax;
	//int i = 0, j = 0;
	double startTime, endTime;
	int comm_size, rank;
	int ROOT = 0;
	int tag = 2;



	startTime = MPI_Wtime();
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    *(data + 1 + (1*columns)) = 11;
    *(data + 1 + (5900*columns)) = 5900;
    *(data + 2 + (5900*columns)) = 25900;
    *(data + 389 + (1*columns)) = 389;
    *(data + 389 + (5900*columns)) = 3895900;
    *(data + 391 + (1*columns)) = 391;
    *(data + 391 + (5900*columns)) = 3915900;
	*(data + 600 + (2*columns)) = 1;
    *(data + 600 + (3003*columns)) = 1;


    int sizeToSend = rows * columns / comm_size;
    float * dataptr = NULL;
    float * data_part = (float*)malloc(sizeToSend * sizeof(float));
    int part_columns = columns / comm_size;
    if(rank == 0) {
        dataptr = data;
    }

    MPI_Datatype column_type, column_wise_array_type, column_type2, column_wise_array_type2;
    if(rank == 0) {
        MPI_Type_vector(rows, 1, columns, MPI_FLOAT, &column_type);
        MPI_Type_commit(&column_type);
        MPI_Type_create_resized(column_type, 0, 1 * sizeof(float), &column_wise_array_type);
        MPI_Type_commit(&column_wise_array_type);
    }

    MPI_Type_vector(rows, 1, part_columns, MPI_FLOAT, &column_type2);
    MPI_Type_commit(&column_type2);
    MPI_Type_create_resized(column_type2, 0, 1 * sizeof(float), &column_wise_array_type2);
    MPI_Type_commit(&column_wise_array_type2);

    MPI_Scatter(dataptr, part_columns, column_wise_array_type,
                data_part, part_columns, column_wise_array_type2,
                0, MPI_COMM_WORLD);



    for (int i = 1; i < part_columns; i++) {
        //std::cout << "rank:" << rank << " i=" << i << std::endl;

        min = 0;
		max = 0;
		for (int j = 0; j < rows; j++)
		{
			if (*(data_part + (j*part_columns)+i) < min)
			{
				min = *(data_part + (j*part_columns)+i);
			}

			if (*(data_part + (j*part_columns)+i) > max)
			{
				max = *(data_part + (j*part_columns)+i);
			}
		}

	//	MPI_Reduce(&min, &globalMin, 1, MPI_INT, MPI_MIN, ROOT, MPI_COMM_WORLD);
	//	MPI_Reduce(&max, &globalMax, 1, MPI_INT, MPI_MAX, ROOT, MPI_COMM_WORLD);

		float max_min_reciprocal = max - min;
		if (max_min_reciprocal == 0) {
			continue;
		}
		max_min_reciprocal = 1. / max_min_reciprocal;

		for (int j = 0; j < rows; j++)
		{
			//printf("Procek %d wykonuje\n ", rank);
            *(data_part + (j*part_columns)+i) = (*(data_part + (j*part_columns)+i) - min) * max_min_reciprocal;
		}
	}

    if (rank == 0) {
        for (int i = 0; i < part_columns; i++)
        {
            for (int j = 0; j < rows; j++) {
                std::cout << "localData:" << *(data_part + (j*part_columns)+i) << " i:" << i << " j:" << j << std::endl;
            }
        }
    }

	printf("Procek %d Finished\n ", rank);
	MPI_Barrier(MPI_COMM_WORLD);
	endTime = MPI_Wtime();

	MPI_Finalize();

	if (rank == 0)
		printf("Czas obliczen Normalizacja: %d\n", endTime - startTime);

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

	startTime = MPI_Wtime();
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int countRows = rows / size;
	int sizeToSend = countRows * columns;
	float *localData = (float*)malloc(countRows * columns * sizeof(float));

	MPI_Scatter(data, sizeToSend, MPI_FLOAT, localData, sizeToSend, MPI_FLOAT,
		ROOT, MPI_COMM_WORLD);

	for (i = 1; i < columns - 1; i++)
	{
	    amo = 0;
        ave = 0;
        var = 0;
        for (int j = 0; j < countRows; j++) {
            amo = amo + *(localData + (j*columns)+i);
        }
		MPI_Reduce(&amo, &globalAmo, 1, MPI_FLOAT, MPI_SUM, ROOT, MPI_COMM_WORLD);
        ave  = amo / float(rows);

        for (int j = 0; j < countRows; j++){
            float factor = *(localData + (j*columns)+i) - ave;
            var = var + (factor * factor);
        }
		MPI_Reduce(&var, &globalVar, 1, MPI_FLOAT, MPI_SUM, ROOT, MPI_COMM_WORLD);

		if (globalVar == 0) {
            for (j = 0; j < countRows; j++) {
                *(data + (j*columns)+i) = *(localData + (j*columns)+i) / 255.;
            }
            continue;
		}

        float sd_reciprocal = 1./sqrt(globalVar);

		for (j = 0; j < countRows; j++) {
            *(data + (j*columns)+i) = (*(localData + (j*columns)+i) - ave) * sd_reciprocal;
		}

	}

	printf("Procek %d Finished\n ", rank);
	//MPI_Barrier(MPI_COMM_WORLD);
	endTime = MPI_Wtime();

	MPI_Finalize();

	if (rank == 0)
		printf("Czas obliczen Normalizacja: %d\n", endTime - startTime);

}


