using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;
using System;
using System.Threading;
using System.Threading.Tasks;

public class SerialInterface
{
    private SerialPort radioSerialPort;
    private string port;
    private int baudrate;
    private SemaphoreSlim semaphore = new SemaphoreSlim(1, 1); // Semaphore to manage access to the serial port
    private Queue<DataPacket> dataQueue = new Queue<DataPacket>(); // Queue to store incoming data
    private bool isReading = false; // Flag to track if a read operation is in progress

    public SerialInterface(string port, int baudrate)
    {
        this.port = port;
        this.baudrate = baudrate;

        radioSerialPort = new SerialPort(port, baudrate);
        radioSerialPort.ReadTimeout = 200;
        radioSerialPort.WriteTimeout = 200;

        radioSerialPort.Open();
    }

    public void SendCommand(Command command){

        byte[] data = BitConverter.GetBytes((int) command);
        radioSerialPort.Write(data, 0, 1);
    }

    // Call this function to start the read operation in the background
    private void StartReading()
    {
        if (!isReading) // Ensure we don't start multiple reads at once
        {
            isReading = true;
            Task.Run(() => ReadSerialPortAsync());
        }
    }

    // This method performs the read operation asynchronously
    private async Task ReadSerialPortAsync()
    {
        try
        {
            await semaphore.WaitAsync(); // Ensure only one thread reads at a time

            bool prevByteFF = false;
            byte[] currByte = { 0x00 };

            radioSerialPort.Read(currByte, 0, 1);

            while (!prevByteFF || currByte[0] != 0xFF)
            {
                if (currByte[0] == 0xFF)
                {
                    prevByteFF = true;
                }
                else
                {
                    prevByteFF = false;
                }

                radioSerialPort.Read(currByte, 0, 1);
            }

            byte[] buf = new byte[32];
            int n = radioSerialPort.Read(buf, 0, 32);

            DataPacket dataPacket = new DataPacket
            {
                dataMask = BitConverter.ToInt32(buf, 0),
                timestamp = BitConverter.ToSingle(buf, 4)
            };

            for (int i = 0; i < 6; i++)
            {
                dataPacket.data[i] = BitConverter.ToSingle(buf, (i * 4) + 8);
            }

            // Push the read data to the queue
            lock (dataQueue)
            {
                dataQueue.Enqueue(dataPacket);
            }
        }
        catch (TimeoutException)
        {
            Debug.LogWarning("Unable to read from serial port: " + port);
        }
        finally
        {
            semaphore.Release(); // Ensure the semaphore is released even if an exception occurs
            isReading = false; // Mark the read as complete
        }
    }

    // Call this function to check if data is ready
    public DataPacket GetData()
    {
        StartReading();

        lock (dataQueue)
        {
            if (dataQueue.Count > 0)
            {
                return dataQueue.Dequeue();
            }
        }
        return null; // No data available yet
    }
}
