#include <iostream>
#include "LoadCSV.h"
#include "DataClass.h"
#include "Preprocessing.h"
#include "KnnAlgorithm.h"

using namespace std;

int main()
{
	DataClass ourData = DataClass();
	LoadCSV loadData = LoadCSV();
	Preprocessing preprocessing = Preprocessing();
	KnnAlgorithm knnAlg = KnnAlgorithm();

	loadData.myLoad(ourData.data, ourData.rows, ourData.columns);
	ourData.data = preprocessing.Normalization(ourData.data, ourData.rows, ourData.columns);
	knnAlg.SplitData(ourData.data, 75);
	float acc = knnAlg.accuracy();
	printf("%f", acc);

	return 0;
}