#include <iostream>
#include "LoadCSV.h"
#include "DataClass.h"
#include "Preprocessing.h"
#include "KnnAlgorithm.h"
#include <stdio.h>
#include <ctime>

using namespace std;

int main()
{
	DataClass ourData = DataClass();
	LoadCSV loadData = LoadCSV();
	Preprocessing preprocessing = Preprocessing();
	KnnAlgorithm knnAlg = KnnAlgorithm();

    int numberOfThreads = 8;
	loadData.myLoad(ourData.data, ourData.rows, ourData.columns);
    preprocessing.Normalization(ourData.data, ourData.rows, ourData.columns, numberOfThreads);
	//preprocessing.Standarization(ourData.data, ourData.rows, ourData.columns, numberOfThreads);
	KnnAlgorithm knn = KnnAlgorithm();
    knn.fit(ourData, 75);
	float acc = knn.predict(numberOfThreads);
    cout << "accuracy: " << acc << endl;

	return 0;
}
