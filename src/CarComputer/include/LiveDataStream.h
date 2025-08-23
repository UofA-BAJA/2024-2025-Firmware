#ifndef LIVEDATASTREAM_H
#define LIVEDATASTREAM_H

#include <iostream>
#include <queue>
#include "DataTypes.h"

namespace BajaWildcatRacing
{

    class LiveDataStream{

        public:
            LiveDataStream(DataTypes dataType);

            void enqueue(float data);
            float dequeue();
            bool dataInQueue();
            void clearAllData();

            DataTypes getDataType();

        private:
            DataTypes dataType;
            float lastVal = 0.0f;

            std::queue<float> dataQueue;

    };
    
}


#endif