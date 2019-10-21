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

	loadData.myLoad(ourDataMnist.data, ourDataMnist.rows, ourDataMnist.columns);
	preprocessing.Normalization(ourDataMnist.data, ourDataMnist.rows, ourDataMnist.columns, 1);
	knnAlg.fit(ourDataMnist, 75);
	float acc = knnAlg.predict();
	printf("%f", acc);


	return 0;
}