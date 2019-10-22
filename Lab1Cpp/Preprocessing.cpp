#include "Preprocessing.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <omp.h>
#include <time.h>
#include <cmath>
#include <omp.h>
#include <time.h>


Preprocessing::Preprocessing()
{
}


Preprocessing::~Preprocessing()
{
}

void Preprocessing::Normalization(float *data, int rows, int columns, int numberOfThreads)
{
	double start, end;
	int min = 0, max = 0;
	int i = 0, j = 0;
	start = omp_get_wtime();

	#pragma omp parallel default(none) private(i, j, min, max) shared(data, rows, columns, numberOfThreads) num_threads(numberOfThreads)
	#pragma omp for schedule(dynamic, numberOfThreads)
	for (int i = 1; i < columns-1; i++)
	{
		min = 0;
		max = 0;
		for (int j = 0; j < rows; j++)
		{
			if (*(data + i + (j*columns)) < min)
			{
				min = *(data + (j*columns)+i);
			}

			if (*(data + (j*columns)+i) > max)
			{
				max = *(data + (j*columns)+i);
			}
		}

        float max_min_reciprocal = max - min;
        if (max_min_reciprocal == 0) {
            continue;
        }
        max_min_reciprocal = 1. / max_min_reciprocal;

		for (int j = 0; j < rows; j++)
		{
            *(data + (j*columns)+i) = (*(data + (j*columns)+i) - min) * max_min_reciprocal;
		}
	}

	end = omp_get_wtime();

	printf("Czas obliczen normalizacja: %f.\n", end - start);
}

void Preprocessing::Standarization(float *data, int rows, int columns, int numberOfThreads)
{
	double start, end;
	int i = 0, j = 0;
    float var = 0, ave = 0, amo=0;

    start = omp_get_wtime();
   // #pragma omp parallel default(none) private(i, j, ave, amo, var) shared(data, rows, columns, numberOfThreads) num_threads(numberOfThreads)
	//#pragma omp for schedule(dynamic, numberOfThreads)
	for (i = 1; i < columns - 1; i++)
	{
	    amo = 0;
        ave = 0;
        var = 0;
        for (int j = 0; j < rows; j++) {
            amo = amo + *(data + (j*columns)+i);
        }
        ave  = amo / float(rows);


        for (int j = 0; j < rows; j++){
            float factor = *(data + (j*columns)+i) - ave;
            var = var + (factor * factor);
        }

		if (var == 0) {
            for (j = 0; j < rows; j++) {
                *(data + (j*columns)+i) = *(data + (j*columns)+i) / 255.;
            }
            continue;
		}

        float sd_reciprocal = 1./sqrt(var);

		for (j = 0; j < rows; j++) {
            *(data + (j*columns)+i) = (*(data + (j*columns)+i) - ave) * sd_reciprocal;
		}

	}

	end = omp_get_wtime();

	printf("Czas obliczen standaryzacja: %f.\n", end - start);
}


