using RoyTheunissen.Graphing;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;
using System;

public class Test : MonoBehaviour
{
    [SerializeField] GraphComponent testGraph;
    SerialPort radioSerialPort;
    private void Awake()
    {
        string port = "/dev/ttyUSB0";
        int baudrate = 115200;

        radioSerialPort = new SerialPort(port, baudrate);

        radioSerialPort.ReadTimeout = 1500;
        radioSerialPort.WriteTimeout = 1500;

        radioSerialPort.Open();
    }
    void Update()
    {
        testGraph.Graph.SetDuration(8f);

        // float rand = RandomGaussian();
        // // testGraph.Graph.AddValue(Mathf.Cos(Time.time * Mathf.PI), "Test value");
        // testGraph.Graph.AddValue(rand, "Test value");


        readSerialPort();

    }

    private void readSerialPort(){
        try{
            string message = radioSerialPort.ReadLine();


            string[] data = message.Split(' ');

            if(data.Length < 5){
                return;
            }
            string yData = data[3];

            try{
                float yVal = float.Parse(yData);
                testGraph.Graph.AddValue(yVal, "Y value");
            }
            catch(FormatException){

            }

            // Debug.Log(message);
        }
        catch(TimeoutException){

        }
    }



    public static float RandomGaussian(float minValue = 0.0f, float maxValue = 1.0f)
    {
        float u, v, S;

        do
        {
            u = 2.0f * UnityEngine.Random.value - 1.0f;
            v = 2.0f * UnityEngine.Random.value - 1.0f;
            S = u * u + v * v;
        }
        while (S >= 1.0f);

        // Standard Normal Distribution
        float std = u * Mathf.Sqrt(-2.0f * Mathf.Log(S) / S);

        // Normal Distribution centered between the min and max value
        // and clamped following the "three-sigma rule"
        float mean = (minValue + maxValue) / 2.0f;
        float sigma = (maxValue - mean) / 3.0f;
        return Mathf.Clamp(std * sigma + mean, minValue, maxValue);
    }
}
