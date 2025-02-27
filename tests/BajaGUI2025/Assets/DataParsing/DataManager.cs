using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DataManager : MonoBehaviour
{

    SerialInterface serialInterface = new SerialInterface("/dev/ttyUSB0", 115200);
    void Start()
    {
        
    }

    void Update()
    {
        DataPacket packet = serialInterface.ReadSerialPort();

        Debug.Log(packet.timestamp);

    }
}
