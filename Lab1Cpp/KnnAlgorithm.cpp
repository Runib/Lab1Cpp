#include "KnnAlgorithm.h"
#include <limits>
#include <cmath>
#include <omp.h>
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
	dataTrainRows = (data.rows * percent)/100;
	dataColumns = data.columns;
	dataTestRows = data.rows - dataTrainRows;
	trainData = data.data;
	testData = data.data + (dataColumns * dataTrainRows);
}

float KnnAlgorithm::predict(int numberOfThreads) {
    int accurate_predictions = 0;
    float max_float = std::numeric_limits<float>::max();
 	double start, end;
    int current_test_row;
    int closest_neighbour_index;
	float closest_neighbour_distance;

    start = omp_get_wtime();
    #pragma omp parallel default(none) private(current_test_row, closest_neighbour_distance, closest_neighbour_index) shared(max_float, numberOfThreads) num_threads(numberOfThreads) reduction(+ : accurate_predictions)
    #pragma omp for schedule(dynamic, numberOfThreads)
    for (int current_test_row=0; current_test_row < dataTestRows; ++current_test_row) {
        float closest_neighbour_distance = max_float;
        // for each row in train dataset

        float* tst = testData + (dataColumns*current_test_row);
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
        if(*(testData+(current_test_row*dataColumns)) == *(trainData+(closest_neighbour_index*dataColumns))) {
            // if prediction is accurate increment accurate predictions counter
            accurate_predictions = accurate_predictions + 1;
        }
    }
    	end = omp_get_wtime();
 	printf("Czas obliczen KnnAlgorytm: %f.\n", end - start);
    return (accurate_predictions/float(dataTestRows))*100;
}
