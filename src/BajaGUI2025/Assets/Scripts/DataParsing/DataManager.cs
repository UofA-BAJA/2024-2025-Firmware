using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading.Tasks;
using RoyTheunissen.Graphing;
using UnityEngine;

public class DataManager : MonoBehaviour
{

    [SerializeField] GraphComponent graph;
    Dictionary<DataType, DataStream> dataStreams = new();

    SerialInterface serialInterface = new SerialInterface("/dev/ttyUSB0", 115200);

    private void Awake()
    {
        for(int i = 0; i < 32; i++){
            dataStreams.Add((DataType)i, new DataStream((DataType)i, "Idk rn let's be so real", "also idk"));
        }

    }

    private void Update()
    {

        DataPacket packet = serialInterface.GetData();

        // Now we have the exciting task of decoding the packets =D
        // The following line helps get a binary representation
        // Debug.Log(Convert.ToString(packet.dataMask, 2));

        if(packet == null){
            return;
        }

        if(packet.dataMask == 0){
            return;
        }

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

        // Example of how to get the data and use it. 

        // if(!dataStreams[DataType.IMU_ROTATION_Y].IsEmpty() && !dataStreams[DataType.CVT_TEMPERATURE].IsEmpty()){
        //     KeyValuePair<float, float> values1 = dataStreams[DataType.IMU_ROTATION_Y].GetOldestData();
        //     KeyValuePair<float, float> values2 = dataStreams[DataType.CVT_TEMPERATURE].GetOldestData();

        //     plot.SetData(values1.Value, values2.Value, Time.time);
        // }

        if(!dataStreams[DataType.IMU_ROTATION_Y].IsEmpty()){
            KeyValuePair<float, float> speed = dataStreams[DataType.IMU_ROTATION_Y].GetOldestData();
            // plot.SetData(speed.Key, speed.Value, 0);

            graph.Graph.AddValue(speed.Value, 0);

        }
    }


    public void SendCommand(Command command){
        serialInterface.SendCommand(command);
    }
}
