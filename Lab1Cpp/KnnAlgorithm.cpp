#include "KnnAlgorithm.h"
#include <limits>
#include <cmath>
#include <mpi.h>
#include <time.h>
#include <stdio.h>

KnnAlgorithm::KnnAlgorithm()
{
    //ctor
}

KnnAlgorithm::~KnnAlgorithm()
{
    //dtor
}

void KnnAlgorithm::fit(DataClass data, int percent) {
    int rank;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    
        dataTrainRows = (data.rows * percent)/100;
	    dataColumns = data.columns;
	    dataTestRows = data.rows - dataTrainRows;
	    trainData = data.data;
	    testData = data.data + (dataColumns * dataTrainRows);

}

float KnnAlgorithm::predict() {
    int accurate_predictions = 0, globalAccuratePredictions;
    float max_float = std::numeric_limits<float>::max();
    int current_test_row;
    int closest_neighbour_index;
	float closest_neighbour_distance;

    double startTime, endTime;
	int size, rank;
	int ROOT = 0;
	int tag = 2;

    MPI_Barrier(MPI_COMM_WORLD);
	startTime = MPI_Wtime();

    MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int countDataTestRows = dataTestRows / size;
	float *localTestData = (float*)malloc(dataColumns * countDataTestRows * sizeof(float));

    MPI_Scatter(testData, countDataTestRows * dataColumns, MPI_FLOAT, localTestData, 
    countDataTestRows * dataColumns, MPI_FLOAT, ROOT, MPI_COMM_WORLD);

    for (int current_test_row=0; current_test_row < countDataTestRows; ++current_test_row) {
        float closest_neighbour_distance = max_float;
        // for each row in train dataset

        float* tst = localTestData + (dataColumns*current_test_row);
        for (int i = 0; i < dataTrainRows; ++i) {
            float* tr = trainData + (i*dataColumns) + 1;
            // calculate eucidlean metric and get the closest one
            float sum = 0;
            for(int j = 1; j < dataColumns; ++j, ++tr) {

                float difference = *(tr) - *(tst +j);
                sum = sum + (difference * difference);
            }
            // distance is euclidlean metric for current_test_row and i-th train data
            // if our data is closer to that row from train data update closest_neighbour_distance and and closest_neighbour_index
            if(sum < closest_neighbour_distance) {
                closest_neighbour_distance = sum;
                closest_neighbour_index = i;
            }
        }
        // now we have found closest neightbour and have index of that neighbour in closest_neighbour_index variable
        // so let's get target class of that neightbour (predicted class) and check if the prediction is accurate
        if(*(localTestData+(current_test_row*dataColumns)) == *(trainData+(closest_neighbour_index*dataColumns))) {
            // if prediction is accurate increment accurate predictions counter
            accurate_predictions = accurate_predictions + 1;
        }
    }

    // MPI_Reduce(&accurate_predictions, &globalAccuratePredictions, 
    //     1, MPI_INT, MPI_SUM, ROOT, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
	endTime = MPI_Wtime();


	if (rank == 0)
		printf("Czas obliczen KnnAlgorithm: %f\n", endTime - startTime);

    return (accurate_predictions/float(dataTestRows))*100;
}
