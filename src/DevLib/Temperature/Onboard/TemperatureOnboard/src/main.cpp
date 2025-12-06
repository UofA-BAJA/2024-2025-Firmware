#include <mcp_can.h>
#include <SPI.h>
#include <Wire.h>
#include <MLX90614.h>
#include <Arduino.h>

const int SPI_CS_PIN = 10; // CS pin for the MLX90614
MCP_CAN CAN(SPI_CS_PIN);  // Create CAN object on CS pin



MLX90614 mlx = MLX90614(MLX90614_BROADCASTADDR);


void setup()
{
  Wire.begin();
  Serial.begin(115200);
  mlx.begin();

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
  CAN.init_Filt(0, 1, 0x00000002);
  CAN.init_Filt(1, 1, 0x00000002);

  CAN.init_Mask(1, 1, 0xFFFFFFFF);
  CAN.init_Filt(2, 1, 0x00000002);
  CAN.init_Filt(3, 1, 0x00000002);
  CAN.init_Filt(4, 1, 0x00000002);
  CAN.init_Filt(5, 1, 0x00000002);



  // Set the MCP2515 to normal mode to start receiving CAN messages
  Serial.println("Setting CAN Normal");
  CAN.setMode(MCP_NORMAL);

  //---------------------------------------------------

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
      float onBoardTemp = mlx.readTemp();
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

        byte tempData[sizeof onBoardTemp];

        memcpy(tempData, &onBoardTemp, sizeof onBoardTemp);

        unsigned long callbackID = 0;
        // 3 bytes for the callback ID
        memcpy(&callbackID, rxBuf, 3);

        byte sendMSG = CAN.sendMsgBuf(callbackID, 1, 4, tempData);

        if(sendMSG != CAN_OK){
          Serial.print("Error Sending Message...");
          Serial.println(sendMSG);
        }
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

