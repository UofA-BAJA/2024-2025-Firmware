#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_Alphanumeric_Display.h>
#include <Servo.h>
#include <mcp_can.h>
#include <SPI.h>

// Constants
const int CAN_CS_PIN = 10;

// Devices
HT16K33 display; // 14 segment
HT16K33 display2;
MCP_CAN CAN(CAN_CS_PIN); // CAN
Servo speed;
Servo rpm;


// Variables

void setup()
{
  // <insert that default comment that gets generated with all arduino projects>
  Serial.begin(115200);
  Serial.println("Dash test r8");


  // Join I2C
  Wire.begin();
  Wire.setWireTimeout(25000U, true);

  //---------------------------------------------------

  // initalize 14 segment and check for display acknowledge
  if (display.begin(0x70, 0x71) == false)
  {
    Serial.println("At least one 14 segment from row 1 did not acknowledge!");
    // while (1)
    //   ;
  }
  if (display2.begin(0x72, 0x73) == false)
  {
    Serial.println("At least one 14 segment from row 2 did not acknowledge!");
    // while (1)
    //   ;
  }
  Serial.println("Display setup complete.");
  display.print("TEST R1");
  display2.print("TEST R2");
  // display.updateDisplay();

  //---------------------------------------------------

  // Initalize Servos
  speed.attach(5);
  rpm.attach(6);

  //---------------------------------------------------

  // Initialize and join CAN
  display.print("CAN INIT");
  byte canInitResult = CAN.begin(MCP_ANY, CAN_100KBPS, MCP_8MHZ);

  if (canInitResult == CAN_OK)
  {
    Serial.println("CAN Init OK!");
  }
  else if (canInitResult == CAN_FAILINIT)
  {

    Serial.println("CAN Init Failed: CAN_FAILINIT");
    display.print("CAN FAIL");
    while (1)
      ;
  }
  else if (canInitResult == CAN_FAILTX)
  {
    Serial.println("CAN Init Failed: CAN_FAILTX");
    display.print("CAN FAIL");
    while (1)
      ;
  }
  else
  {
    Serial.println("CAN Init Failed: Unknown error");
    display.print("CAN FAIL");
    while (1)
      ;
  }

  CAN.init_Mask(0, 0, 0x7FF); // Set mask for filter 0 (standard 11-bit ID)
  CAN.init_Filt(0, 0, 0x123); // Accept messages with CAN ID 0x123 (final ID TBD)

  // Set the MCP2515 to normal mode to start receiving CAN messages
  CAN.setMode(MCP_NORMAL);

  // delay(2000);
  display.print("INIT OK");
  delay(700);
  display.print("NO MSG");
  display2.print("WAITING");
  Serial.println("Init Ok!");
}

void loop()
{
  // Handle Can RX
  long unsigned int rxId;
  unsigned char len = 0;
  unsigned char rxBuf[8];

  if (CAN_MSGAVAIL == CAN.checkReceive())
  {

    while (CAN_MSGAVAIL == CAN.checkReceive())
    {
      CAN.readMsgBuf(&rxId, &len, rxBuf); // Read message
      // Serial.print(">RX ID: ");

      // Serial.println(rxId, HEX);
      display.print("RX");
      display2.print(rxId);

      // for (int i = 0; i < len; i++)
      // {
      //   Serial.print(">RX Data: ");
      // Serial.println(rxBuf[i], HEX);
      // }
    }
    // Serial.println(" reached end of buffer -------------------------");
  }

  if (Wire.getWireTimeoutFlag())
  {
    Serial.println("WIRE TIMEOUT FLAG SET!!!");
  }
}
