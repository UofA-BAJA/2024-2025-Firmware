#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_Alphanumeric_Display.h>
#include <Servo.h>
#include <mcp_can.h>
#include <SPI.h>

// Constants
const int CAN_CS_PIN = 10;

// Devices
HT16K33 display;         // 14 segment
MCP_CAN CAN(CAN_CS_PIN); // CAN
Servo speed;
Servo rpm;

// Variables

void setup()
{
  // <insert that default comment that gets generated with all arduino projects>
  Serial.begin(9600);
  Serial.println("Dash test r6");

  // Join I2C
  Wire.begin();

  //---------------------------------------------------

  // initalize 14 segment and check for display acknowledge
  if (display.begin(0x70, 0x71) == false)
  {
    Serial.println("At least one 14 segment did not acknowledge! Freezing.");
    while (1)
      ;
  }
  Serial.println("Display acknowledged.");
  display.print("DISPTEST");
  // display.updateDisplay();

  //---------------------------------------------------

  // Initalize Servos
  speed.attach(9);
  rpm.attach(10);

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
}
void loop()
{
  // Handle Can RX
  long unsigned int rxId;
  unsigned char len = 0;
  unsigned char rxBuf[8];

  if (CAN_MSGAVAIL == CAN.checkReceive())
  {

    CAN.readMsgBuf(&rxId, &len, rxBuf); // Read message
    Serial.print(">ID: ");

    Serial.println(rxId, HEX);
    display.print("RX" + rxId);

    // for (int i = 0; i < len; i++) {
    //   Serial.print(">Data: ");
    //   Serial.println(rxBuf[i], HEX);
    // }

    Serial.println("-------------------------");

    // Handle Command Response (TX)
    //  if(rxBuf[1] == 0xA4){

    //   byte sendMSG = CAN.sendMsgBuf(rxBuf[0],0,8,data);

    //   // if(sendMSG == CAN_OK){
    //   //   Serial.println("Message Sent Successfully!");
    //   // } else {
    //   //   Serial.println("Error Sending Message...");
    //   // }
    // }
  }
  byte error = CAN.checkError();
  if (error == CAN_CTRLERROR)
  {
    Serial.println("CAN CTRL ERROR");
    Serial.println(error, BIN);
  }
  else if (error == CAN_FAIL)
  {
    Serial.println("CAN FAIL");
    Serial.println(error, BIN);
  }
  else if (error == CAN_FAILTX)
  {
    Serial.println("CAN FAIL TX");
    Serial.println(error, BIN);
  }
  else if (error == CAN_OK)
  {
    Serial.println("CAN OK (explicit)");
    Serial.println(error, BIN);
  }
  else
  {
    Serial.print("Check Error: ");
    Serial.println(error, BIN);
  }
  delay(300);
}
