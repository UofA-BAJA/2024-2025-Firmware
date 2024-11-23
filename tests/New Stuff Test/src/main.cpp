#include <Arduino.h>
#include <SparkFun_Alphanumeric_Display.h>
#include <mcp_can.h>
#include <SPI.h>

// Constants
const int CAN_CS_PIN = 10;

// Devices
MCP_CAN CAN(CAN_CS_PIN); // CAN
// Variables

void setup()
{
  // <insert that default comment that gets generated with all arduino projects>
  Serial.begin(115200);
  Serial.println("AMong US test R1");

  //---------------------------------------------------
  //---------------------------------------------------

  // Initialize and join CAN

  byte canInitResult = CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ);

  if (canInitResult == CAN_OK)
  {
    Serial.println("CAN Init OK!");
  }
  else if (canInitResult == CAN_FAILINIT)
  {

    Serial.println("CAN Init Failed: CAN_FAILINIT");
    while (1)
      ;
  }
  else if (canInitResult == CAN_FAILTX)
  {
    Serial.println("CAN Init Failed: CAN_FAILTX");
    while (1)
      ;
  }
  else
  {
    Serial.println("CAN Init Failed: Unknown error");
    while (1)
      ;
  }

  // CAN.init_Mask(0, 0, 0x7FF); // Set mask for filter 0 (standard 11-bit ID)
  // CAN.init_Filt(0, 0, 0x123); // Accept messages with CAN ID 0x123 (final ID TBD)

  // Set the MCP2515 to normal mode to start receiving CAN messages
  CAN.setMode(MCP_NORMAL);

  delay(700);
  Serial.println("Init Ok!");
}

void loop()
{
  // // Handle Can RX
  // long unsigned int rxId;
  // unsigned char len = 0;
  // unsigned char rxBuf[8];


  //   while (CAN_MSGAVAIL == CAN.checkReceive())
  //   {
  //     CAN.readMsgBuf(&rxId, &len, rxBuf); // Read message
  //   }


 
}
