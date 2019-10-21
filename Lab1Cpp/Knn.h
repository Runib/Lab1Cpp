#ifndef KNN_H
#define KNN_H
#include "DataClass.h"

class Knn
{
    public:
        Knn();
        virtual ~Knn();
        void fit(DataClass data, int percent);
        float predict();
        float **trainData;
        float **testData;
        int dataTrainRows;
        int dataTestRows;
        int dataColumns;
    protected:
    private:
};

#endif // KNN_H
