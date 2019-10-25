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
	int i = 0, j = 0;
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

	for (int i = 1; i < columns-1; i++)
	{
		min = 0;
		max = 0;
		for (int j = 0; j < countRows; j++)
		{
			if (*(localData + i + (j*columns)) < min)
			{
				min = *(localData + (j*columns)+i);
			}

			if (*(localData + (j*columns)+i) > max)
			{
				max = *(localData + (j*columns)+i);
			}
		}

		MPI_Reduce(&min, &globalMin, 1, MPI_INT, MPI_MIN, ROOT, MPI_COMM_WORLD);
		MPI_Reduce(&max, &globalMax, 1, MPI_INT, MPI_MAX, ROOT, MPI_COMM_WORLD);

		float max_min_reciprocal = globalMax - globalMin;
		if (max_min_reciprocal == 0) {
			continue;
		}
		max_min_reciprocal = 1. / max_min_reciprocal;

		for (int j = 0; j < countRows; j++)
		{
			//printf("Procek %d wykonuje\n ", rank);
			*(data + (j*columns)+i) = (*(localData + (j*columns)+i) - globalMin) * max_min_reciprocal;
		}
	}

	printf("Procek %d Finished\n ", rank);
	//MPI_Barrier(MPI_COMM_WORLD);
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


