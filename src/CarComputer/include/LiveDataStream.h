#ifndef LIVEDATASTREAM_H
#define LIVEDATASTREAM_H


#include <queue>
#include "DataTypes.h"


class LiveDataStream{

    public:
        LiveDataStream(DataTypes dataType);

        void Enqueue(float data);
        float Dequeue();

        DataTypes getDataType();

    private:
        DataTypes dataType;

        std::queue<float> dataQueue;

};

#endif