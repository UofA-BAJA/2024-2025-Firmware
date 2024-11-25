#include <mcp_can.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

const int SPI_CS_PIN = 5; // CS pin for the MCP2515
MCP_CAN CAN(SPI_CS_PIN);  // Create CAN object on CS pin

Adafruit_BNO055 bno = Adafruit_BNO055(55);

 
float xRot = 0;
float yRot = 0;
float zRot = 0;
float xAcc = 0;
float yAcc = 0;
float zAcc = 0;

SemaphoreHandle_t bnoMutex = NULL;

void readBNO(void *pvParameters);


void setup()
{
  Serial.begin(115200);
  byte canInitResult = CAN.begin(MCP_STDEXT, CAN_500KBPS, MCP_8MHZ);

  if (canInitResult == CAN_OK)
  {
    Serial.println("CAN Init OK!");
  }
  else if (canInitResult == CAN_FAILINIT)
  {
    Serial.println("CAN Init Failed: CAN_FAILINIT");
    Serial.end();
    while (1)
      ;
  }
  else if (canInitResult == CAN_FAILTX)
  {
    Serial.println("CAN Init Failed: CAN_FAILTX");
    Serial.end();
    while (1)
      ;
  }
  else
  {
    Serial.println("CAN Init Failed: Unknown error");
    Serial.end();
    while (1)
      ;
  }

  CAN.init_Mask(0, 1, 0xFFFFFFFF);
  CAN.init_Filt(0, 1, 0x00000001);
  CAN.init_Filt(1, 1, 0x00000001);

  CAN.init_Mask(1, 1, 0xFFFFFFFF);
  CAN.init_Filt(2, 1, 0x00000001);
  CAN.init_Filt(3, 1, 0x00000001);
  CAN.init_Filt(4, 1, 0x00000001);
  CAN.init_Filt(5, 1, 0x00000001);

  // Set the MCP2515 to normal mode to start receiving CAN messages
  Serial.println("Setting CAN Normal");
  CAN.setMode(MCP_NORMAL);

  //---------------------------------------------------


  // Create mutex O.o crazy comment right here
  bnoMutex = xSemaphoreCreateMutex();

  xTaskCreate(
    readBNO,
    "Read BNO",
    4096,
    NULL,
    10,
    NULL
  );

  delay(1000);

 Serial.println("Init OK!");
}

// byte data[8] = {0x01, 0x01, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};


void loop()
{
  long unsigned int rxId = 0;
  unsigned char len = 0;
  unsigned char rxBuf[8];

  rxBuf[0] = 0;
  rxBuf[1] = 0;

  // Check for incoming CAN messages
  while (CAN_MSGAVAIL == CAN.checkReceive())
  {

    if(xSemaphoreTake(bnoMutex, portMAX_DELAY)){
        
      CAN.readMsgBuf(&rxId, &len, rxBuf); // Read message
      // Serial.print(">ID: ");
      // Serial.println(rxId, HEX);

      // for (int i = 0; i < len; i++) {
      //   Serial.print(">Data: ");
      //   Serial.println(rxBuf[i], HEX);
      // }

      // Serial.println("-------------------------");
      if (rxBuf[3] == 0x01)
      {

        byte xData[sizeof xRot];

        memcpy(xData, &xRot, sizeof xRot);

        unsigned long callbackID = 0;
        // 3 bytes for the callback ID
        memcpy(&callbackID, rxBuf, 3);
        byte sendMSG = CAN.sendMsgBuf(callbackID, 1, 4, xData);

        if(sendMSG != CAN_OK){
          Serial.print("Error Sending Message...");
          Serial.println(sendMSG);
        }
      }
      else if (rxBuf[3] == 0x02)
      {

        byte yData[sizeof yRot];

        memcpy(yData, &yRot, sizeof yRot);

        unsigned long callbackID = 0;
        // 3 bytes for the callback ID
        memcpy(&callbackID, &rxBuf[0], 3);

        byte sendMSG = CAN.sendMsgBuf(callbackID, 1, 4, yData);

        if(sendMSG != CAN_OK){
          Serial.print("Error Sending Message...");
          Serial.println(sendMSG);
        }
      }
      else if (rxBuf[3] == 0x03)
      {

        byte zData[sizeof zRot];

        memcpy(zData, &zRot, sizeof zRot);

        unsigned long callbackID = 0;
        // 3 bytes for the callback ID
        memcpy(&callbackID, &rxBuf[0], 3);
        byte sendMSG = CAN.sendMsgBuf(callbackID, 1, 4, zData);

        if(sendMSG != CAN_OK){
          Serial.print("Error Sending Message...");
          Serial.println(sendMSG);
        }
      }
      else if (rxBuf[3] == 0x04)
      {

        byte zData[sizeof xAcc];

        memcpy(zData, &xAcc, sizeof xAcc);

        unsigned long callbackID = 0;
        // 3 bytes for the callback ID
        memcpy(&callbackID, &rxBuf[0], 3);

        byte sendMSG = CAN.sendMsgBuf(callbackID, 1, 4, zData);

        if(sendMSG != CAN_OK){
          Serial.print("Error Sending Message...");
          Serial.println(sendMSG);
        }
      }
      else if (rxBuf[3] == 0x05)
      {

        byte zData[sizeof yAcc];

        memcpy(zData, &yAcc, sizeof yAcc);

        unsigned long callbackID = 0;
        // 3 bytes for the callback ID
        memcpy(&callbackID, &rxBuf[0], 3);

        byte sendMSG = CAN.sendMsgBuf(callbackID, 1, 4, zData);

        if(sendMSG != CAN_OK){
          Serial.print("Error Sending Message...");
          Serial.println(sendMSG);
        }
      }
      else if (rxBuf[3] == 0x06)
      {

        byte zData[sizeof zAcc];

        memcpy(zData, &zAcc, sizeof zAcc);

        unsigned long callbackID = 0;
        // 3 bytes for the callback ID
        memcpy(&callbackID, &rxBuf[0], 3);

        byte sendMSG = CAN.sendMsgBuf(callbackID, 1, 4, zData);

        if(sendMSG != CAN_OK){
          Serial.print("Error Sending Message...");
          Serial.println(sendMSG);
        }
      }

      xSemaphoreGive(bnoMutex);
    }
  }
  // Serial.println("end of buffer");
  // byte canErrorCount = CAN.errorCountRX();
  // if(canErrorCount > 0){
  // Serial.print("ERROR: ");
  //   Serial.println(CAN.getError(), BIN);
  // }
  // Serial.print("RECIEVE");
  // Serial.println(CAN, BIN);
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
  // i++;
}

void readBNO(void *pvParameters){

  /* Initialise the sensor */
  Serial.println("BNO Init");
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1)
      ;
  }
  // bno.setMode(OPERATION_MODE_GYRONLY);

  bno.setExtCrystalUse(true);
  Serial.println("BNO Init OK!");
  Wire.setClock(400000);

  while(true){

    

      sensors_event_t event;
      sensors_event_t accelerationEvent;
    
      bno.getEvent(&event);
      bno.getEvent(&accelerationEvent, Adafruit_BNO055::VECTOR_LINEARACCEL);
    if(xSemaphoreTake(bnoMutex, portMAX_DELAY)){
      xRot = event.orientation.x;
      yRot = event.orientation.y;
      zRot = event.orientation.z;
      xAcc = accelerationEvent.acceleration.x;
      yAcc = accelerationEvent.acceleration.y;
      zAcc = accelerationEvent.acceleration.z;
      xSemaphoreGive(bnoMutex);
    }

    delay(30);
  }

}