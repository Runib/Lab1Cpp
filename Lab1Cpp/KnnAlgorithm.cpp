#include "KnnAlgorithm.h"
#include <limits>
#include <cmath>
#include <omp.h>
#include <time.h>
#include <stdio.h>

KnnAlgorithm::KnnAlgorithm()
{
}

KnnAlgorithm::~KnnAlgorithm()
{
}


void KnnAlgorithm::fit(DataClass data, int percent) {
	dataTrainRows = (data.rows * percent) / 100;
	dataColumns = data.columns;
	dataTestRows = data.rows - dataTrainRows;
	trainData = &data.data[0];
	testData = &data.data[dataTrainRows];
}

float KnnAlgorithm::predict() {
	int accurate_predictions = 0;
	float max_float = std::numeric_limits<float>::max();
	double start, end;
	int numberOfThreads = 1, i, current_test_row, j, closest_neighbour_index;;
	float sum, distance;
	float closest_neighbour_distance;

	start = omp_get_wtime();

	#pragma omp parallel default(none) private(current_test_row, closest_neighbour_distance, distance) \
	 shared(numberOfThreads, dataTestRows, dataColumns, trainData, testData) num_threads(numberOfThreads) \
	reduction(+ : accurate_predictions)
	for (current_test_row = 0; current_test_row < dataTestRows; current_test_row++) {
		closest_neighbour_distance = max_float;
		// for each row in train dataset
		for (i = 0; i < dataTrainRows; i++) {

			// calculate eucidlean metric and get the closest one
			sum = 0;
			for (j = 1; j < dataColumns; j++) {
				float difference = trainData[i][j] - testData[current_test_row][j];
				sum += difference * difference;
			}
			// distance is euclidlean metric for current_test_row and i-th train data
			distance = sqrt(sum);
			// if our data is closer to that row from train data update closest_neighbour_distance and and closest_neighbour_index
			if (distance < closest_neighbour_distance) {
				closest_neighbour_distance = distance;
				closest_neighbour_index = i;
			}
		}
		// now we have found closest neightbour and have index of that neighbour in closest_neighbour_index variable
		// so let's get target class of that neightbour (predicted class) and check if the prediction is accurate
		if (testData[current_test_row][0] == trainData[closest_neighbour_index][0]) {
			// if prediction is accurate increment counter o accurate predictions
			accurate_predictions += 1;
		}
	}

	end = omp_get_wtime();

	printf("Czas obliczen normalizacja: %f.\n", end - start);

	return (accurate_predictions / float(dataTestRows)) * 100;
}