#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_Alphanumeric_Display.h>
#include <ESP32Servo.h>
#include <mcp_can.h>
#include <SPI.h>
#include <Adafruit_LEDBackpack.h>
#include <Preferences.h>

//Methods
void writeDisplays(void *pvParameters);
void displayCVTTemp(HT16K33 disp);
void displayTime(HT16K33 disp);
void displayRPM(HT16K33 disp);
void displaySpeed(HT16K33 disp);

// Constants
const int CAN_CS_PIN = 5;
const int DISPLAY1_BUTTON_PIN = 12;
const int DISPLAY2_BUTTON_PIN = 14;
enum displayOptions{
  CVT_TEMP = 0,
  TIME,
  RPM,
  SPEED,
  END_ELEMENT
};
const String version = "V1-1-0"; //Will I use this?
const int SPEED_MIN_ANGLE = 176;
const int SPEED_MAX_ANGLE = 0;
const int RPM_MIN_ANGLE = 5;
const int RPM_MAX_ANGLE = 180;

// Devices
HT16K33 display; // 14 segment
HT16K33 display2;
MCP_CAN CAN(CAN_CS_PIN); // CAN
Servo speed;
Servo rpm;
Adafruit_LEDBackpack ledMatrix = Adafruit_LEDBackpack();

SemaphoreHandle_t canMutex = NULL;
Preferences prefs;

// Variables
float lastSpeed = 0;
float lastRPM = 0;
float lastCVTTemp = 0;
float lastTimeSeconds = 0;
uint16_t indicatorLightState = 0;

bool canRecentRX = true;
unsigned long lastCANRx = 0;

int display1CurrentDisplay = 0;
unsigned long lastDisp1Button = 0; //Last button press time
int display2CurrentDisplay = 1;
unsigned long lastDisp2Button = 0; //Last button press time


void setup()
{
  // <insert that default comment that gets generated with all arduino projects>
  delay(3000);
  Serial.begin(115200);
  Serial.print("Dash Firmware ");
  Serial.println(version);

  // Join I2C
  Wire.begin();

  //---------------------------------------------------

  // initalize 14 segment and check for display acknowledge
  if (display.begin(0x70, 0x71) == false)
  {
    Serial.println("[WARNING] At least one 14 segment from row 1 did not acknowledge!");
  }
  if (display2.begin(0x72, 0x73) == false)
  {
    Serial.println("[WARNING] At least one 14 segment from row 2 did not acknowledge!");
  }
  Serial.println("Display setup complete.");

  //---------------------------------------------------

  // Initalize Servos
  speed.attach(32);
  rpm.attach(33);
  Serial.println("Servos initialized.");

  //---------------------------------------------------

  // Initialize LED Matrix
  if (ledMatrix.begin(0x74) == false)
  {
    Serial.println("[WARNING] LED Matrix Driver did not acknowledge!");
    // while (1)
    //   ;
  }
  Serial.println("LED Matrix setup complete.");

  //---------------------------------------------------

  //Light Test
  ledMatrix.displaybuffer[0] = 0b1111111111111111;
  ledMatrix.displaybuffer[1] = 0b1111111111111111;
  ledMatrix.writeDisplay();
  //Define an all-on character and print it out, then turn on decimal and colon
  display.defineChar('`', 0b1111111111111111);
  display2.defineChar('`', 0b1111111111111111);
  display.print("````````");
  display2.print("````````");
  display.colonOn();
  display2.colonOn();
  display.decimalOn();
  display2.decimalOn();
  //Sweep the servos
  delay(200); //Pause for stabilize?
  speed.write(SPEED_MAX_ANGLE);
  rpm.write(RPM_MAX_ANGLE);
  delay(1000);
  speed.write(SPEED_MIN_ANGLE);
  rpm.write(RPM_MIN_ANGLE);
  delay(1000);
  //Clear displays
  display.clear();
  display2.clear();
  ledMatrix.clear();
  ledMatrix.writeDisplay();
  display.print("WELCOME");
  display2.print("TO BAJA");
  delay(1500);
  Serial.println("Light test complete.");
  
  //---------------------------------------------------

  // Initialize and join CAN
  byte canInitResult = CAN.begin(MCP_STDEXT, CAN_500KBPS, MCP_8MHZ);

  if (canInitResult == CAN_OK)
  {
    Serial.println("CAN Init OK!");
  }
  else if (canInitResult == CAN_FAILINIT)
  {

    Serial.println("[ERROR] CAN Init Failed: CAN_FAILINIT");
    display.print("CAN FAIL");
    display2.print("FAILINIT");
    while (1)
      ;
  }
  //There used to be a block for CAN_FAILTX here, but I removed it as the only options are CAN_FAILINIT and CAN_OK
  else
  {
    Serial.println("[ERROR] CAN Init Failed: Unknown error");
    display.print("CAN FAIL");
    display2.print("UNKNOWN");
    while (1)
      ;
  }

  //Init filter so we only worry about our CAN ID
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
  Serial.println("CAN setup complete");

  //---------------------------------------------------

  //Initialize pins for buttons
  pinMode(DISPLAY1_BUTTON_PIN, INPUT_PULLUP);
  pinMode(DISPLAY2_BUTTON_PIN, INPUT_PULLUP);

  //---------------------------------------------------

  //Load Display preferences
  prefs.begin("DisplayPrefs", false);
  if(!prefs.isKey("Display1") || !prefs.isKey("Display2")){
    //Put in "Factory Defaults" otherwise it will error (example: swapping ESP32s)
    prefs.putInt("Display1", display1CurrentDisplay);
    prefs.putInt("Display2", display2CurrentDisplay);
  }else{
    //Load in whatever's stored 
    display1CurrentDisplay = prefs.getInt("Display1");
    display2CurrentDisplay = prefs.getInt("Display2");
  }
  prefs.end();

  //---------------------------------------------------

  // Create mutex (absolutely insane comment)
  canMutex = xSemaphoreCreateMutex();
  xTaskCreate(
    writeDisplays,
    "Write Displays",
    4096,
    NULL, 
    10,
    NULL
  );

  //Init OK!
  display2.clear();
  display.print("INIT OK");
  display2.print(version);
  Serial.println("Init Finished!");
  delay(1500);

}



void loop()
{
  //Variables for CAN RX
  unsigned long rxId;
  unsigned char len = 0;
  unsigned char rxBuf[8];

  while (CAN_MSGAVAIL == CAN.checkReceive()) {
    //Read CAN message buffer
    CAN.readMsgBuf(&rxId, &len, rxBuf); 
    // Serial.println(rxId, HEX);
    if(xSemaphoreTake(canMutex, portMAX_DELAY)){
      //Print out data for debugging
      // Serial.print(">RX Data: ");
      // for (int i = 0; i < len; i++)
      // {
      //   Serial.print(rxBuf[i], HEX);
      //   Serial.print(" ");
      // } 

      //Copy the data into the right location in memory 
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
          memcpy(&lastTimeSeconds, &rxBuf[1], sizeof(float));
          break;
        case 0x05:
          // Indicator Lights
          memcpy(&indicatorLightState, &rxBuf[1], sizeof(uint16_t));
          break;
      }
      canRecentRX = true;
      xSemaphoreGive(canMutex);
      lastCANRx = millis();
    }
  }

  //If we haven't recieved a CAN command in 2 seconds, assume CAN is offline
  if(millis()-lastCANRx > 2000 && canRecentRX){
      if(xSemaphoreTake(canMutex, portMAX_DELAY)){
        canRecentRX = false;
        xSemaphoreGive(canMutex);
      }
      //Light up the appropriate indicator light
      //TODO: I don't think this is the right one anymore, fix it
      ledMatrix.displaybuffer[0] = indicatorLightState | 1;
      ledMatrix.displaybuffer[1] = indicatorLightState | 1;
      ledMatrix.writeDisplay();
  }
}



void writeDisplays(void *pvParameters){
  delay(1500);
  int num = 0; //cycle counter for arbitrary delay
  while(true){
    num++;

    //thread safety for this variable
    bool tempCANRecentRX = true; 
    if(xSemaphoreTake(canMutex, portMAX_DELAY)){
      tempCANRecentRX = canRecentRX;
      xSemaphoreGive(canMutex); 
    }

    // If there has not been can recently, flash NO CAN
    if(num%8==0 && !tempCANRecentRX){
      ledMatrix.displaybuffer[0] = indicatorLightState | 1;
      ledMatrix.displaybuffer[1] = indicatorLightState | 1;
      ledMatrix.writeDisplay();
      if(num%16 == 0){
        display.clear();
        // ledMatrix.displaybuffer[0] = 0b1111111111111111;
        // ledMatrix.displaybuffer[1] = 0;
        // ledMatrix.writeDisplay();
      }else{
        display.print("NO CAN");
        speed.write(((20 / 38.0) * (SPEED_MAX_ANGLE - SPEED_MIN_ANGLE)) + SPEED_MIN_ANGLE);
        rpm.write(((2000 / 3950.0) * (RPM_MAX_ANGLE - RPM_MIN_ANGLE)) + RPM_MIN_ANGLE);
        // ledMatrix.displaybuffer[1] = 0b1111111111111111;
        // ledMatrix.displaybuffer[0] = 0;
        // ledMatrix.writeDisplay();
      }
    }

    //If there has been CAN recently, display stuff 
    if(tempCANRecentRX){
      //Switch display data field with cooldown (note: active low)
      if(digitalRead(DISPLAY1_BUTTON_PIN) == LOW && millis()-lastDisp1Button > 300){
        lastDisp1Button = millis();
        display1CurrentDisplay++;
        //Rollover
        if(display1CurrentDisplay == END_ELEMENT){
          display1CurrentDisplay = 0;
        }
        //Save to the ESP32
        prefs.begin("DisplayPrefs", false);
        prefs.putInt("Display1", display1CurrentDisplay);
        prefs.end();
      }
      if(digitalRead(DISPLAY2_BUTTON_PIN) == LOW && millis()-lastDisp2Button > 300){
        lastDisp2Button = millis();
        display2CurrentDisplay++;
        //Rollover
        if(display2CurrentDisplay == END_ELEMENT){
          display2CurrentDisplay = 0;
        }
        //Save to the ESP32
        prefs.begin("DisplayPrefs", false);
        prefs.putInt("Display2", display2CurrentDisplay);
        prefs.end();
      }

      //Display the correct thing for the display
      switch(display1CurrentDisplay){
        case CVT_TEMP:
          displayCVTTemp(display);
          break;
        case TIME:
          displayTime(display);
          break;
        case RPM:
          displayRPM(display);
          break;
        case SPEED:
          displaySpeed(display);
          break;
        default:
          display.print("????");
          break;
      }

      switch(display2CurrentDisplay){
        case CVT_TEMP:
          displayCVTTemp(display2);
          break;
        case TIME:
          displayTime(display2);
          break;
        case RPM:
          displayRPM(display2);
          break;
        case SPEED:
          displaySpeed(display2);
          break;
        default:
          display2.print("????");
          break;
      }

      //Write LED matrix and RPM/Speed gauges
      if(xSemaphoreTake(canMutex, portMAX_DELAY)){
        //Copy everything to new variables so the mutex can be given as fast as possible
        uint16_t tempIndicatorLights = indicatorLightState; 
        float tempSpeed = lastSpeed;
        float tempRPM = lastRPM;
        // float tempSpeed = 20;
        // float tempRPM = 2000;
        xSemaphoreGive(canMutex);
        ledMatrix.displaybuffer[0] = tempIndicatorLights;
        ledMatrix.displaybuffer[1] = tempIndicatorLights;
        ledMatrix.writeDisplay(); 
        //Formula: ((value/maxValue) * servoRange) + minRange
        speed.write(((tempSpeed / 36.0) * (SPEED_MAX_ANGLE - SPEED_MIN_ANGLE)) + SPEED_MIN_ANGLE);
        rpm.write(((tempRPM / 3950.0) * (RPM_MAX_ANGLE - RPM_MIN_ANGLE)) + RPM_MIN_ANGLE);
      }
    }
    //Effectively limits the refresh rate to ~10Hz (which is fine, we don't need much more than 5Hz)
    delay(100); 
  } 
}

//Displays the latest CVT temp information on the given display
void displayCVTTemp(HT16K33 disp){
  if(xSemaphoreTake(canMutex, portMAX_DELAY)){
    float tempCVTTemp = lastCVTTemp;
    xSemaphoreGive(canMutex);
    disp.printf("CVT %4.0f", tempCVTTemp);
  }
}

//Displays the latest time information on the given display
void displayTime(HT16K33 disp){
  if(xSemaphoreTake(canMutex, portMAX_DELAY)){
    unsigned long realSeconds = (unsigned long)lastTimeSeconds;
    xSemaphoreGive(canMutex);
    unsigned long currentMinutes = lastTimeSeconds/60;
    unsigned long currentHours = lastTimeSeconds/3600;
    disp.printf("%02u%02u  %02u", currentHours, currentMinutes%60, realSeconds%60);  
    disp.colonOn();
  }
}

//Displays the latest RPM information on the given display
void displayRPM(HT16K33 disp){
  if(xSemaphoreTake(canMutex, portMAX_DELAY)){
    float tempRPM = lastRPM;
    xSemaphoreGive(canMutex);
    disp.printf("RPM %4.0f", tempRPM);
  }
}

//Displays the latest speed information on the given display
void displaySpeed(HT16K33 disp){
  if(xSemaphoreTake(canMutex, portMAX_DELAY)){
    float tempSpeed = lastSpeed;
    xSemaphoreGive(canMutex);
    disp.printf("SPD %4.0f", tempSpeed);
  }
}