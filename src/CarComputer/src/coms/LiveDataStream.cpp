#include "LiveDataStream.h"

namespace BajaWildcatRacing
{

    LiveDataStream::LiveDataStream(DataTypes dataType){
        this->dataType = dataType;
    }


    void LiveDataStream::enqueue(float data){
        dataQueue.push(data);
    }

    float LiveDataStream::dequeue(){

        if(dataQueue.empty()){
            return 0;
        }

        float data = dataQueue.front();

        dataQueue.pop();
        return data;
    }

    void LiveDataStream::clearAllData(){
        std::queue<float> empty;
        std::swap(dataQueue, empty); 
    }

    bool LiveDataStream::dataInQueue(){
        return !dataQueue.empty();
    }

    DataTypes LiveDataStream::getDataType(){
        return this->dataType;
    }
    
}
