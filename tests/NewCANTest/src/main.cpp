//new can test

#include <mcp_can.h>
#include <SPI.h>
#include <Wire.h>
// #include <MLX90614.h>
#include <Arduino.h>
#include "CANTProtocol.h"



const int SPI_CS_PIN = 10; // CS pin for the MLX90614
// MCP_CAN CAN(SPI_CS_PIN);  // Create CAN object on CS pin

CANTProtocol CAN(SPI_CS_PIN, 2, 0x1Fl);

typedef struct TestStruct_s{
    float a;
    float b;
    float c;
    float d;
    float e;
} TestStruct;

// MLX90614 mlx = MLX90614(MLX90614_BROADCASTADDR);

void builderA(unsigned char dataLength, byte* incomingData, unsigned long callbackID){
    TestStruct bruh = {69.0, 70.0, 71.0, 72.0, 73.0};
    CAN.sendRequestResponse(bruh, callbackID);
}

void builderB(unsigned char dataLength, byte* incomingData, unsigned long callbackID){

}


void setup()
{
    Wire.begin();
    Serial.begin(115200);

    bool requestReg = CAN.registerRequest(0x01, builderA);
    if(!requestReg) Serial.println("Request registriation failed!!!");

    requestReg = CAN.registerCommand(0x02, builderB);
    if(!requestReg) Serial.println("Command registriation failed!!!");

    bool canInitResult = CAN.begin();


    if(!canInitResult){
        Serial.println("CAN Init failed!!!!!");
        while (1) ;
    }
    

    //---------------------------------------------------

    delay(1000);

    Serial.println("Init OK!");
}


void loop()
{
    CAN.execute();      
}

