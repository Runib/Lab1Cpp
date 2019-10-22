#include <iostream>
#include "LoadCSV.h"
#include "DataClass.h"
#include "Preprocessing.h"
#include "KnnAlgorithm.h"
#include "Knn.h"
#include <stdio.h>
#include <ctime>

using namespace std;

int main()
{
	DataClass ourData = DataClass();
	LoadCSV loadData = LoadCSV();
	Preprocessing preprocessing = Preprocessing();
	KnnAlgorithm knnAlg = KnnAlgorithm();

    //freopen( "log.txt", "w", stdout );
    int numberOfThreads = 1;
	loadData.myLoad(ourData.data, ourData.rows, ourData.columns);
    //ourData.print_data();
    //preprocessing.Normalization(ourData.data, ourData.rows, ourData.columns, numberOfThreads);
	preprocessing.Standarization(ourData.data, ourData.rows, ourData.columns, numberOfThreads);
    //ourData.print_data();
	KnnAlgorithm knn = KnnAlgorithm();
    knn.fit(ourData, 75);
    clock_t begin = clock();
	float acc = knn.predict(numberOfThreads);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "accuracy: " << acc << endl;
    cout << "elapsed time: " << elapsed_secs << endl;

	printf("%f", acc);

	return 0;
}
