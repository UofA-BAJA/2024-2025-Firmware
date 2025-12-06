#include <Arduino.h>
#include <mcp_can.h>


const int SPI_CS_PIN = 10;
MCP_CAN CAN(SPI_CS_PIN);

// // Define the analog pin and constant
const int analogPin0 = A0;  // Analog input pin connected to the sensor
const int analogPin1 = A1; 
const int analogPin2 = A2; 
const float voltageReference = 5.0;  // Reference voltage for the ADC

float getRPM0();
float getRPM1();
float getRPM2();

void setup() {
    Serial.begin(115200);  // Initialize serial communication
    pinMode(analogPin0, INPUT);  // Set the analog pin as an input
    pinMode(analogPin1, INPUT);  // Set the analog pin as an input
    pinMode(analogPin2, INPUT);  // Set the analog pin as an input

    byte canInitResult = CAN.begin(MCP_STDEXT,CAN_500KBPS, MCP_8MHZ);
    
    if (canInitResult == CAN_OK)
  {
    Serial.println("CAN Init OK!");
  }
  else if (canInitResult == CAN_FAILINIT)
  {
    Serial.println("CAN Init Failed: CAN_FAILINIT");
    Serial.end();
    while (1);
  }
  else if (canInitResult == CAN_FAILTX)
  {
    Serial.println("CAN Init Failed: CAN_FAILTX");
    Serial.end();
    while (1);
  }
  else
  {
    Serial.println("CAN Init Failed: Unknown error");
    Serial.end();
    while (1);

    delay(500);
    Serial.println("Init OK!");
}

CAN.init_Mask(0, 1, 0xFFFFFFFF);
  CAN.init_Filt(0, 1, 0x00000005);
  CAN.init_Filt(1, 1, 0x00000005);

  CAN.init_Mask(1, 1, 0xFFFFFFFF);
  CAN.init_Filt(2, 1, 0x00000005);
  CAN.init_Filt(3, 1, 0x00000005);
  CAN.init_Filt(4, 1, 0x00000005);
  CAN.init_Filt(5, 1, 0x00000005);

  // Set the MCP2515 to normal mode to start receiving CAN messages
  Serial.println("Setting CAN Normal");
  CAN.setMode(MCP_NORMAL);

 

  delay(500);

 Serial.println("Init OK!");

}


void loop() {

long unsigned int rxId = 0;
unsigned char len = 0;
unsigned char rxBuf[8];

rxBuf[0] = 0;
rxBuf[1] = 0;

//check for incoming CAN messages
while (CAN_MSGAVAIL == CAN.checkReceive())
{

    CAN.readMsgBuf(&rxId, &len, rxBuf);


    if (rxBuf[3]==0x01)
    {
        float wheelRPM = getRPM0();
        byte wheelRPMData[sizeof wheelRPM];
        
        memcpy(wheelRPMData, &wheelRPM, sizeof wheelRPM);

        Serial.println(wheelRPM);

        unsigned long callbackID = 0;
        // 3 bytes for the callback ID
        memcpy(&callbackID, rxBuf, 3);

        byte sendMSG = CAN.sendMsgBuf(callbackID,1 ,4,wheelRPMData);

        if (sendMSG != CAN_OK)
        {
            Serial.print("Error Sending Message...");
            Serial.println(sendMSG);
        }
        
    }
    else if (rxBuf[3]==0x02)
    {
      float wheelRPM = getRPM1();
      byte wheelRPMData[sizeof wheelRPM];
      
      memcpy(wheelRPMData, &wheelRPM, sizeof wheelRPM);

      Serial.println(wheelRPM);

      unsigned long callbackID = 0;
      // 3 bytes for the callback ID
      memcpy(&callbackID, rxBuf, 3);

      byte sendMSG = CAN.sendMsgBuf(callbackID,1 ,4,wheelRPMData);

      if (sendMSG != CAN_OK)
      {
          Serial.print("Error Sending Message...");
          Serial.println(sendMSG);
      }
        
    }

    else if (rxBuf[3]==0x03)
    {
      float wheelRPM = getRPM2();
      byte wheelRPMData[sizeof wheelRPM];
      
      memcpy(wheelRPMData, &wheelRPM, sizeof wheelRPM);

      Serial.println(wheelRPM);

      unsigned long callbackID = 0;
      // 3 bytes for the callback ID
      memcpy(&callbackID, rxBuf, 3);

      byte sendMSG = CAN.sendMsgBuf(callbackID,1 ,4,wheelRPMData);

      if (sendMSG != CAN_OK)
      {
          Serial.print("Error Sending Message...");
          Serial.println(sendMSG);
      }
        
    }
    

  }
}

float getRPM0() {
    int analogValue = analogRead(analogPin0);
    float voltage = (analogValue / 1023.0) * voltageReference;
    float rpm = 134 * voltage;  // Adjust the equation as needed for your application
    return rpm;
}

float getRPM1() {
  int analogValue = analogRead(analogPin1);
  float voltage = (analogValue / 1023.0) * voltageReference;
  float rpm = 134 * voltage;  // Adjust the equation as needed for your application
  return rpm;
}

float getRPM2() {
  int analogValue = analogRead(analogPin2);
  float voltage = (analogValue / 1023.0) * voltageReference;
  float rpm = 134 * voltage;  // Adjust the equation as needed for your application
  return rpm;
}