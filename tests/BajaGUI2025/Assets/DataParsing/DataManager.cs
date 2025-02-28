using System;
using System.Collections;
using System.Collections.Generic;
using RoyTheunissen.Graphing;
using UnityEngine;

public class DataManager : MonoBehaviour
{
    [SerializeField] GraphComponent testGraph;

    Dictionary<DataType, DataStream> dataStreams = new();

    SerialInterface serialInterface = new SerialInterface("/dev/ttyUSB0", 115200);

    private void Awake()
    {
        for(int i = 0; i < 32; i++){
            dataStreams.Add((DataType)i, new DataStream((DataType)i, "Idk rn let's be so real", "also idk"));
        }

        Debug.Log(dataStreams.ContainsKey(DataType.CAR_SPEED));
        testGraph.Graph.SetDuration(8f);
    }
    private void Start()
    {
        
    }

    private void Update()
    {
        DataPacket packet = serialInterface.ReadSerialPort();

        // Now we have the exciting task of decoding the packets =D
        // The following line helps get a binary representation
        // Debug.Log(Convert.ToString(packet.dataMask, 2));


        int dataMask = packet.dataMask;

        int dataIndex = 0;
        for(int i = 0; i < 32; i++){
            
            if((dataMask & 1) == 1){

                dataStreams[(DataType)i].AddDataToStream(packet.timestamp, packet.data[dataIndex]);
                dataIndex++;
            }

            if(dataIndex == 6){
                break;
            }

            dataMask >>= 1;
        }

        // Example of how to get the data

        if(!dataStreams[DataType.CAR_SPEED].IsEmpty()){
            KeyValuePair<float, float> values = dataStreams[DataType.CAR_SPEED].GetOldestData();
            testGraph.Graph.AddValue(values.Value, "idk");

        }


    }
}
