using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using RoyTheunissen.Graphing;
using UnityEngine;

public class DataManager : MonoBehaviour
{
    public static event Action<List<DataType>> AvailableDataTypesChanged;

    // [SerializeField] private GraphComponent graph;
    private Dictionary<DataType, DataStream> dataStreams = new();

    // Maps the data type to the time since data of that type was last received, in seconds
    private Dictionary<DataType, float> availableDataTypes = new();
    private SerialInterface serialInterface = new("/dev/ttyUSB0", 115200);

    public static DataManager Instance {get; private set;}

    private void Awake()
    {

        if(Instance == null){
            Instance = this;
        }
        else{
            Destroy(gameObject);
        }


        for(int i = 0; i < 32; i++){
            dataStreams.Add((DataType)i, new DataStream((DataType)i, "Idk rn let's be so real", "also idk"));
        }

    }


    private void Update()
    {

        // I don't really want to iterate through each data type each frame, but I can't think of an easy alternative
        // at the moment.

        foreach(DataType dataType in availableDataTypes.Keys.ToList()){

            if(availableDataTypes[dataType] > .5f){
                availableDataTypes.Remove(dataType);
                AvailableDataTypesChanged?.Invoke(GetAvaiableDataTypes());
            }
            else{
                availableDataTypes[dataType] += Time.deltaTime;
            }
        }

        
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

                DataType dataType = (DataType) i;

                if(!availableDataTypes.ContainsKey(dataType)){
                    AvailableDataTypesChanged?.Invoke(GetAvaiableDataTypes());
                }

                availableDataTypes[dataType] = 0f;

                dataStreams[dataType].AddDataToStream(packet.timestamp, packet.data[dataIndex]);
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

        // if(!dataStreams[DataType.CVT_TEMPERATURE].IsEmpty()){
        //     KeyValuePair<float, float> speed = dataStreams[DataType.CVT_TEMPERATURE].GetOldestData();
        //     // plot.SetData(speed.Key, speed.Value, 0);

        //     graph.Graph.AddValue(speed.Value, 0);

        // }
    }

    // void LateUpdate()
    // {
        
    // }

    // Issue here: we're currently dequeuing the data, so if two things ask for this data, only one will get it per frame
    public KeyValuePair<float, float> GetLatestData(DataType dataType){
        if(dataStreams[dataType].IsEmpty()){
            throw new ArgumentException("No data for DataType: " + dataType.ToString());
        }

        return dataStreams[dataType].GetOldestData();
    }

    public void SendCommand(Command command){
        serialInterface.SendCommand(command);
    }

    public List<DataType> GetAvaiableDataTypes(){
        return new List<DataType>(availableDataTypes.Keys);
    }

}
