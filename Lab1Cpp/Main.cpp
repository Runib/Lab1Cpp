#include <iostream>
#include "LoadCSV.h"
#include "DataClass.h"
#include "Preprocessing.h"
#include "KnnAlgorithm.h"
#include <stdio.h>
#include <ctime>
#include <mpi.h>

using namespace std;

int main()
{
	int rank;

	DataClass ourData = DataClass();
	LoadCSV loadData = LoadCSV();
	Preprocessing preprocessing = Preprocessing();
	KnnAlgorithm knnAlg = KnnAlgorithm();
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0)
		loadData.myLoad(ourData.data, ourData.rows, ourData.columns);
	//ourData.print_data();
    preprocessing.Normalization(ourData.data, ourData.rows, ourData.columns);
	//urData.print_data();
	// //preprocessing.Standarization(ourData.data, ourData.rows, ourData.columns);
	KnnAlgorithm knn = KnnAlgorithm();
    knn.fit(ourData, 75);
	float acc = knn.predict();
    cout << "accuracy: " << acc << endl;
	MPI_Finalize();

	return 0;
}
