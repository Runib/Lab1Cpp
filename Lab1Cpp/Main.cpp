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
	loadData.myLoad(ourData.data, ourData.rows, ourData.columns);
    //ourData.print_data();
    //ourData.data = preprocessing.Normalization(ourData.data, ourData.rows, ourData.columns);
	ourData.data = preprocessing.Standarization(ourData.data, ourData.rows, ourData.columns);
    //ourData.print_data();
	Knn knn = Knn();
    knn.fit(ourData, 75);
    clock_t begin = clock();
	float acc = knn.predict();
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "accuracy: " << acc << endl;
    cout << "elapsed time: " << elapsed_secs << endl;

	printf("%f", acc);

	return 0;
}
