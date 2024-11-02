#include <mcp_can.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


const int SPI_CS_PIN = 10;  // CS pin for the MCP2515
MCP_CAN CAN(SPI_CS_PIN);    // Create CAN object on CS pin

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup() {
  Serial.begin(115200);
  byte canInitResult = CAN.begin(MCP_ANY, CAN_100KBPS, MCP_8MHZ);

  if (canInitResult == CAN_OK) {
    Serial.println("CAN Init OK!");
  } else if (canInitResult == CAN_FAILINIT) {
    Serial.println("CAN Init Failed: CAN_FAILINIT");
    while(1);
  } else if (canInitResult == CAN_FAILTX) {
    Serial.println("CAN Init Failed: CAN_FAILTX");
    while(1);
  } else {
    Serial.println("CAN Init Failed: Unknown error");
    while(1);
  }


  // I don't believe the masking is currently working.
  CAN.init_Mask(0, 0, 0x7FF);  // Set mask for filter 0 (standard 11-bit ID)
  CAN.init_Filt(0, 0, 0x123);  // Accept messages with CAN ID 0x123

  // Set the MCP2515 to normal mode to start receiving CAN messages
  CAN.setMode(MCP_NORMAL);

  //---------------------------------------------------

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
    
  bno.setExtCrystalUse(true);
}

// byte data[8] = {0x01, 0x01, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

void loop() {
      sensors_event_t event; 
      bno.getEvent(&event);

      // Serial.print("X: ");
      // Serial.print(event.orientation.x, 4);
      // Serial.println();

    long unsigned int rxId = 0;
    unsigned char len = 0;
    unsigned char rxBuf[8];

    rxBuf[0] = 0;
    rxBuf[1] = 0;

  // Check for incoming CAN messages
    if(CAN_MSGAVAIL == CAN.checkReceive()) {
      CAN.readMsgBuf(&rxId, &len, rxBuf);  // Read message
      // Serial.print(">ID: ");
      // Serial.println(rxId, HEX);

      // for (int i = 0; i < len; i++) {
      //   Serial.print(">Data: ");
      //   Serial.println(rxBuf[i], HEX);
      // }

      // Serial.println("-------------------------");
      if(rxBuf[3] == 0x01){

        byte xData[sizeof event.orientation.x];

        memcpy(xData, &event.orientation.x, sizeof event.orientation.x);

        unsigned long callbackID = 0;
        // 3 bytes for the callback ID
        memcpy(&callbackID, rxBuf, 3);

        byte sendMSG = CAN.sendMsgBuf(callbackID, 1, 4, xData);

        // if(sendMSG == CAN_OK){
        //   Serial.println("Message Sent Successfully! Z");
        // } else {
        //   Serial.println("Error Sending Message...");
        // }
      }
      else if(rxBuf[3] == 0x02){

        byte yData[sizeof event.orientation.y];

        memcpy(yData, &event.orientation.y, sizeof event.orientation.y);

        unsigned long callbackID = 0;
        // 3 bytes for the callback ID
        memcpy(&callbackID, &rxBuf[0], 3);

        byte sendMSG = CAN.sendMsgBuf(callbackID, 1, 4, yData);
    
        // if(sendMSG == CAN_OK){
        //   Serial.println("Message Sent Successfully! Y");
        // } else {
        //   Serial.println("Error Sending Message...");
        // }
      }
      else if(rxBuf[3] == 0x03){

        byte zData[sizeof event.orientation.z];

        memcpy(zData, &event.orientation.z, sizeof event.orientation.z);

        unsigned long callbackID = 0;
        // 3 bytes for the callback ID
        memcpy(&callbackID, &rxBuf[0], 3);
        byte sendMSG = CAN.sendMsgBuf(callbackID, 1, 4, zData);

        // if(sendMSG == CAN_OK){
        //   Serial.println("Message Sent Successfully! Z");
        // } else {
        //   Serial.println("Error Sending Message...");
        // }
      }
      else if(rxBuf[3] == 0x04){

        byte zData[sizeof event.acceleration.x];

        memcpy(zData, &event.acceleration.x, sizeof event.acceleration.x);

        unsigned long callbackID = 0;
        // 3 bytes for the callback ID
        memcpy(&callbackID, &rxBuf[0], 3);
        byte sendMSG = CAN.sendMsgBuf(callbackID, 1, 4, zData);

        // if(sendMSG == CAN_OK){
        //   Serial.println("Message Sent Successfully! A-X");
        // } else {
        //   Serial.println("Error Sending Message...");
        // }
      }
      else if(rxBuf[3] == 0x05){

        byte zData[sizeof event.acceleration.y];

        memcpy(zData, &event.acceleration.y, sizeof event.acceleration.y);

        unsigned long callbackID = 0;
        // 3 bytes for the callback ID
        memcpy(&callbackID, &rxBuf[0], 3);
        byte sendMSG = CAN.sendMsgBuf(callbackID, 1, 4, zData);

        // if(sendMSG == CAN_OK){
        //   Serial.println("Message Sent Successfully! A-Y");
        // } else {
        //   Serial.println("Error Sending Message...");
        // }
      }
      else if(rxBuf[3] == 0x06){

        byte zData[sizeof event.acceleration.z];

        memcpy(zData, &event.acceleration.z, sizeof event.acceleration.z);

        unsigned long callbackID = 0;
        // 3 bytes for the callback ID
        memcpy(&callbackID, &rxBuf[0], 3);
        byte sendMSG = CAN.sendMsgBuf(callbackID, 1, 4, zData);

        // if(sendMSG == CAN_OK){
        //   Serial.println("Message Sent Successfully! A-Z");
        // } else {
        //   Serial.println("Error Sending Message...");
        // }
      }
      
    }


        /* Get a new sensor event */ 
      
      /* Display the floating point data */
      // Serial.print("X: ");
      // Serial.print(event.orientation.x, 4);
      // Serial.print("\tY: ");
      // Serial.print(event.orientation.y, 4);
      // Serial.print("\tZ: ");
      // Serial.print(event.orientation.z, 4);
      // Serial.println("");
    // _delay_ms(10);
}