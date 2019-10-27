#include <iostream>
#include "LoadCSV.h"
#include "DataClass.h"
#include "Preprocessing.h"
#include "KnnAlgorithm.h"
#include <stdio.h>
#include <ctime>

using namespace std;
/**
 * Run in terminal using following command:
 * mpirun -np 4 cmake-build-debug/Lab1Cpp
 * where number after -np option specify number of processes
 * @return
 */
int main()
{
    freopen( "log.txt", "w", stdout );
	DataClass ourData = DataClass();
	LoadCSV loadData = LoadCSV();
	Preprocessing preprocessing = Preprocessing();
	KnnAlgorithm knnAlg = KnnAlgorithm();
	loadData.myLoad(ourData.data, ourData.rows, ourData.columns);
	//ourData.print_data();
	cout << "Normalization" << endl;
    preprocessing.Normalization(ourData.data, ourData.rows, ourData.columns);
	//ourData.print_data();
	// //preprocessing.Standarization(ourData.data, ourData.rows, ourData.columns);
	// KnnAlgorithm knn = KnnAlgorithm();
    // knn.fit(ourData, 75);
	// float acc = knn.predict(;
    // cout << "accuracy: " << acc << endl;

	return 0;
}
