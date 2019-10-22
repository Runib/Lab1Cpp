#pragma once
#include "DataClass.h"

class KnnAlgorithm
{
    public:
        KnnAlgorithm();
        virtual ~KnnAlgorithm();
        void fit(DataClass data, int percent);
        float predict(int numberOfThreads);
        float *trainData;
        float *testData;
        int dataTrainRows;
        int dataTestRows;
        int dataColumns;
    protected:
    private:
};
