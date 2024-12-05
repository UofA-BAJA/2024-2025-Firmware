#include "LiveDataStream.h"



LiveDataStream::LiveDataStream(DataTypes dataType){
    this->dataType = dataType;
}


void LiveDataStream::Enqueue(float data){
    dataQueue.push(data);
}

float LiveDataStream::Dequeue(){
    float data = dataQueue.front();
    dataQueue.pop();
    return data;
}