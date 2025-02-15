#ifndef LIVEDATASTREAM_H
#define LIVEDATASTREAM_H


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

            DataTypes getDataType();

        private:
            DataTypes dataType;

            std::queue<float> dataQueue;

    };
    
}


#endif