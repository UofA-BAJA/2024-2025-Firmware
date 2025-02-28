using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;
using System;
using Unity.VisualScripting;

public class SerialInterface
{
    private SerialPort radioSerialPort;

    private string port;
    private int baudrate;
    public SerialInterface(string port, int baudrate){

        this.port = port;
        this.baudrate = baudrate;

        radioSerialPort = new SerialPort(port, baudrate);

        radioSerialPort.ReadTimeout = 1500;
        radioSerialPort.WriteTimeout = 1500;

        radioSerialPort.Open();
    }


    public DataPacket ReadSerialPort(){

        DataPacket dataPacket = new();


        try{

            // In between each packet there are two bytes of FF. We have to make sure we 
            // read those before we read a packet, otherwise reading packets is going to
            // be offset incorrectly.
            bool prevByteFF = false;
            byte[] currByte = {0x00};
            radioSerialPort.Read(currByte, 0, 1);


            while(!prevByteFF || currByte[0] != 0xFF){

                if(currByte[0] == 0xFF){
                    prevByteFF = true;
                }
                else{
                    prevByteFF = false;
                }

                radioSerialPort.Read(currByte, 0, 1);

            }


            byte[] buf = new byte[32];
            int n = radioSerialPort.Read(buf, 0, 32);

            dataPacket.dataMask = BitConverter.ToInt32(buf, 0);
            dataPacket.timestamp = BitConverter.ToSingle(buf, 4);

            for(int i = 0; i < 6; i++){
                dataPacket.data[i] = BitConverter.ToSingle(buf, (i * 4) + 8);
            }

        }
        catch(TimeoutException){

        }

        return dataPacket;
    }
}
