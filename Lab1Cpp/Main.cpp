#include <iostream>
#include "LoadCSV.h"
#include "DataClass.h"
#include "Preprocessing.h"
#include "KnnAlgorithm.h"

using namespace std;

int main()
{
	DataClass ourDataMnist = DataClass();
	LoadCSV loadData = LoadCSV();
	Preprocessing preprocessing = Preprocessing();
	KnnAlgorithm knnAlg = KnnAlgorithm();

	int numberOfThreads = 1;
	loadData.myLoad(ourDataMnist.data, ourDataMnist.rows, ourDataMnist.columns);
	preprocessing.Normalization(ourDataMnist.data, ourDataMnist.rows, ourDataMnist.columns, numberOfThreads);
	knnAlg.fit(ourDataMnist, 75);
	float acc = knnAlg.predict(numberOfThreads);
	printf("%f", acc);


	return 0;
}