#include <Arduino.h>
#include <mcp_can.h>


const int SPI_CS_PIN = 10;
MCP_CAN CAN(SPI_CS_PIN  );

// Define the analog pin and constant
const int analogPin = A0;  // Analog input pin connected to the sensor
const float voltageReference = 5.0;  // Reference voltage for the ADC
//const float k = 1000.0;  // Calibration constant for MPH calculation

float getMPH();

void setup() {

    Serial.begin(115200);  // Initialize serial communication
    pinMode(analogPin, INPUT);  // Set the analog pin as an input

    byte canInitResult = CAN.begin(MCP_STDEXT,CAN_500KBPS, MCP_8MHZ);

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
  CAN.init_Filt(0, 1, 0x00000004);
  CAN.init_Filt(1, 1, 0x00000004);

  CAN.init_Mask(1, 1, 0xFFFFFFFF);
  CAN.init_Filt(2, 1, 0x00000004);
  CAN.init_Filt(3, 1, 0x00000004);
  CAN.init_Filt(4, 1, 0x00000004);
  CAN.init_Filt(5, 1, 0x00000004);


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
        float engineMPH = getMPH();
        byte engineMPHData[sizeof engineMPH];
        
        memcpy(engineMPHData, &engineMPH, sizeof engineMPH);

        // Serial.println(engineMPH);

        unsigned long callbackID = 0;
        // 3 bytes for the callback ID
        memcpy(&callbackID, rxBuf, 3);

        byte sendMSG = CAN.sendMsgBuf(callbackID,1 ,4,engineMPHData);

        if (sendMSG != CAN_OK)
        {
            Serial.print("Error Sending Message...");
            Serial.println(sendMSG);
        }
        
    }
    

}

// getMPH();






}

float getMPH(){
    // Read the analog value (0 to 1023)
    int analogValue = analogRead(analogPin);
    
    // Convert the analog value to a voltage (0.0V to 5.0V)
    float voltage = (analogValue / 1023.0) * voltageReference;
    
    // Calculate the shaft speed using the equation:
    // line of best fit for mph: mph = = -70.424x2 + 1279.5x - 210.92

    float mph = 8.75 * voltage;  // Adjust the equation as needed for your application

    // Output the result to the Serial Monitor
    // Serial.print("Voltage: ");
    // Serial.print(voltage, 2);  // Print voltage with 2 decimal places
    // Serial.print(" V, MPH: ");
    // Serial.println(mph, 2);  // Print MPH with 2 decimal places
    return mph;
}