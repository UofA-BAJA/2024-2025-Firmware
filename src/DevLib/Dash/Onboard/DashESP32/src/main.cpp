#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_Alphanumeric_Display.h>
#include <ESP32Servo.h>
#include <mcp_can.h>
#include <SPI.h>
#include <Adafruit_LEDBackpack.h>

void readCAN(void *pvParameters);
void displayCVTTemp();
void displayTime();

// Constants
const int CAN_CS_PIN = 5;

// Devices
HT16K33 display; // 14 segment
HT16K33 display2;
MCP_CAN CAN(CAN_CS_PIN); // CAN
Servo speed;
Servo rpm;
Adafruit_LEDBackpack ledMatrix = Adafruit_LEDBackpack();

SemaphoreHandle_t canMutex = NULL;

// Variables
float lastSpeed = 0;
float lastRPM = 0;
float lastCVTTemp = 0;
unsigned long lastTimeSeconds = 0;
uint16_t indicatorLightState = 0;

bool canRecentRX = false;

void setup()
{
  // <insert that default comment that gets generated with all arduino projects>
  Serial.begin(115200);
  Serial.println("Dash test r11");

  // Join I2C
  Wire.begin();

  //---------------------------------------------------

  // initalize 14 segment and check for display acknowledge
  if (display.begin(0x70, 0x71) == false)
  {
    Serial.println("At least one 14 segment from row 1 did not acknowledge!");
  }
  if (display2.begin(0x72, 0x73) == false)
  {
    Serial.println("At least one 14 segment from row 2 did not acknowledge!");
  }
  Serial.println("Display setup complete.");
  display.print("WELCOME");
  display2.print("TO BAJA");
  delay(1500);

  //---------------------------------------------------

  // Initalize Servos
    speed.attach(32);
    rpm.attach(33);

  //---------------------------------------------------

  // Initialize LED Matrix
  if (ledMatrix.begin(0x74) == false)
  {
    Serial.println("LED Matrix Driver did not acknowledge!");
    // while (1)
    //   ;
  }

  //---------------------------------------------------

  // Initialize and join CAN
  byte canInitResult = CAN.begin(MCP_STDEXT, CAN_500KBPS, MCP_8MHZ);

  if (canInitResult == CAN_OK)
  {
    Serial.println("CAN Init OK!");
  }
  else if (canInitResult == CAN_FAILINIT)
  {

    Serial.println("CAN Init Failed: CAN_FAILINIT");
    display.print("CAN FAIL");
    display2.print("FAILINIT");
    while (1)
      ;
  }
  else if (canInitResult == CAN_FAILTX)
  {
    Serial.println("CAN Init Failed: CAN_FAILTX");
    display.print("CAN FAIL");
    display2.print("FAIL TX");
    while (1)
      ;
  }
  else
  {
    Serial.println("CAN Init Failed: Unknown error");
    display.print("CAN FAIL");
    display2.print("UNKNOWN");
    while (1)
      ;
  }

  CAN.init_Mask(0, 1, 0xFFFFFFFF);
  CAN.init_Filt(0, 1, 0x00000003);
  CAN.init_Filt(1, 1, 0x00000003);

  CAN.init_Mask(1, 1, 0xFFFFFFFF);
  CAN.init_Filt(2, 1, 0x00000003);
  CAN.init_Filt(3, 1, 0x00000003);
  CAN.init_Filt(4, 1, 0x00000003);
  CAN.init_Filt(5, 1, 0x00000003);

  // Set the MCP2515 to normal mode to start receiving CAN messages
  CAN.setMode(MCP_NORMAL);

  // Create mutex (absolutely insane comment)
  canMutex = xSemaphoreCreateMutex();

  xTaskCreate(
    readCAN,
    "Read CAN",
    4096,
    NULL, 
    10,
    NULL
  );

  display2.clear();
  display.print("INIT OK");
  Serial.println("Init Ok!");
  delay(1500);
}

int num = 0;
void loop()
{
  num++;

  if(num%10==0 && !canRecentRX){
    if(num%20 == 0){
      display.clear();
    }else{
     
      display.print("NO CAN");
    }
  }
  if(canRecentRX){
    if(xSemaphoreTake(canMutex, portMAX_DELAY)){
      unsigned long currentMinutes = lastTimeSeconds/60;
      unsigned long currentHours = lastTimeSeconds/3600;
      display.printf("%02u%02u  %02u", currentHours, currentMinutes%60, lastTimeSeconds%60);  
      display.colonOn();
      display2.printf("CVT %4.0f", lastCVTTemp);
      // display2.printf("IMUX%4.0f", lastRPM);
      xSemaphoreGive(canMutex);
    }
  }

  //test led drive and servos
  // if(num%20000==0){
  //   if(num%40000 == 0){
  //     ledMatrix.displaybuffer[5] = 0b0000001000000000;
  //     ledMatrix.writeDisplay();
  //     speed.write(20);
  //     rpm.write(120);
  //   }else{
  //     ledMatrix.displaybuffer[5] = 0b0000000100000000;
  //     ledMatrix.writeDisplay();
  //     speed.write(120);
  //     rpm.write(20);
  //   }
  // }


  delay(50);
}



void readCAN(void *pvParameters){
  // Handle Can RX
  unsigned long rxId;
  unsigned char len = 0;
  unsigned char rxBuf[8];
  unsigned long cyclesWithoutRx = 0;
  while(true){
    while (CAN_MSGAVAIL == CAN.checkReceive()) {
      CAN.readMsgBuf(&rxId, &len, rxBuf); // Read message
      if(xSemaphoreTake(canMutex, portMAX_DELAY)){
        // Serial.print(">RX Data: ");
        // for (int i = 0; i < len; i++)
        // {
        //   Serial.print(rxBuf[i], HEX);
        //   Serial.print(" ");
        // }
        Serial.println();
        switch (rxBuf[0])
        {
          case 0x01:
            // Speed
            memcpy(&lastSpeed, &rxBuf[1], sizeof(float));
            break;
          case 0x02:
            // RPM
            memcpy(&lastRPM, &rxBuf[1], sizeof(float));
            break;
          case 0x03:
            // CVT Temp
            memcpy(&lastCVTTemp, &rxBuf[1], sizeof(float));
            break;
          case 0x04:
            // Timer (seconds)
            memcpy(&lastTimeSeconds, &rxBuf[1], sizeof(unsigned long));
            break;
          case 0x05:
            // Indicator Lights
            memcpy(&indicatorLightState, &rxBuf[1], sizeof(uint16_t));
            ledMatrix.displaybuffer[5] = indicatorLightState;
            ledMatrix.writeDisplay();
            break;
        }
        xSemaphoreGive(canMutex);
      }
      canRecentRX = true;
      cyclesWithoutRx = 0;
    }
    cyclesWithoutRx++;
    if(cyclesWithoutRx == 150){
      canRecentRX = false;
      ledMatrix.displaybuffer[5] = indicatorLightState | 1;
      ledMatrix.writeDisplay();
    }
    delay(20);
  }
  
}

void displayCVTTemp(){

}

void displayTime(){

}