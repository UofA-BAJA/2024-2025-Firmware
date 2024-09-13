#include <mcp_can.h>
#include <SPI.h>

const int SPI_CS_PIN = 10;  // CS pin for the MCP2515
MCP_CAN CAN(SPI_CS_PIN);    // Create CAN object on CS pin

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

  // Set the MCP2515 to normal mode to start receiving CAN messages
  CAN.setMode(MCP_NORMAL);
}

void loop() {
  long unsigned int rxId;
  unsigned char len = 0;
  unsigned char rxBuf[8];

  // Check for incoming CAN messages
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&rxId, &len, rxBuf);  // Read message
    Serial.print(">ID: ");
    Serial.println(rxId, HEX);

    for (int i = 0; i < len; i++) {
      Serial.print(">Data: ");
      Serial.println(rxBuf[i], HEX);
    }

    Serial.println("-------------------------");
  }
}
