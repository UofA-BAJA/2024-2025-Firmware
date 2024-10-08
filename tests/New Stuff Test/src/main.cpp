#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_Alphanumeric_Display.h>

HT16K33 display;

int pointerStart = 0;
int pointerEnd = 34;
String targetString = "THE COMMS BROKE";
String currentString = "";

void setup()
{
  targetString = targetString + "    ";
  pointerEnd = targetString.length();
  currentString = targetString + targetString;

  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("14 segment test r5");

  Wire.begin(); // Join I2C bus

  // check if display will acknowledge
  if (display.begin(0x70, 0x71) == false)
  {
    Serial.println("Device did not acknowledge! Freezing.");
    while (1)
      ;
  }
  Serial.println("Display acknowledged.");

  display.updateDisplay();
}
void loop()
{

  if (Serial.available() > 0)
  {
    String teststr = Serial.readStringUntil('.'); // read until timeout
    teststr.trim();
    targetString = teststr + "    ";
    pointerEnd = targetString.length();
    pointerStart = 0;
    currentString = targetString + targetString;
  }

  display.print(currentString.substring(pointerStart, pointerStart + 8));
  pointerStart++;
  if (pointerStart >= pointerEnd)
    pointerStart = 0;
  delay(200);
}
