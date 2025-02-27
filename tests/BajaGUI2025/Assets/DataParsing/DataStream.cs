using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DataStream
{
    private Queue<KeyValuePair<float, float>> dataInStream = new();

    public DataType DataType {get; private set;}
    public string DataTypeName {get; private set;}
    public string DataUnit {get; private set;}

    public DataStream(DataType dataType, string dataTypeName, string dataUnit){
        this.DataType = dataType;
        this.DataTypeName = dataTypeName;
        this.DataUnit = dataUnit;
    }
    
    public void AddDataToStream(float data, float timestamp){
        dataInStream.Enqueue(new KeyValuePair<float, float>(data, timestamp));
    }

    public KeyValuePair<float, float> GetOldestData(){
        // I mean technically this is an escaping reference but it literally does not affect anything
        // because the data is removed from the queue
        return dataInStream.Dequeue();
    }

}
