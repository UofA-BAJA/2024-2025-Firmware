using System;
using System.IO.Ports;
using System.Text.RegularExpressions;

class Program{



    static SerialPort radioSerialPort;
    public static void Main(String[] args){

        String port = "/dev/ttyUSB0";
        int baudrate = 115200;

        radioSerialPort = new SerialPort(port, baudrate);

        radioSerialPort.ReadTimeout = 1500;
        radioSerialPort.WriteTimeout = 1500;

        radioSerialPort.Open();

        // Test to parse Commands.h file 





        while(true){
            Read();
        }

    }

    private static void Read(){
        try{
            string message = radioSerialPort.ReadLine();
            Console.WriteLine(message);
        }
        catch(TimeoutException){

        }
    }

}