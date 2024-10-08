#include <Arduino.h>

// put function declarations here:
void hallEffectInterrupt();

const byte interruptPin = 3;
const float numMagnets = 5.0f;

volatile bool state = false;
volatile unsigned long lastTime = 0;
volatile unsigned long lastDeltaTime = 1;
volatile float lastRPM = 0;
void setup()
{
  // put your setup code here, to run once:
  pinMode(interruptPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), hallEffectInterrupt, RISING);
  Serial.begin(9600);
}

void loop()
{
  // put your main code here, to run repeatedly:a
  lastRPM = 1.0f / 60.0f * numMagnets * (float)lastDeltaTime;
  Serial.println(lastRPM);

  digitalWrite(LED_BUILTIN, state);
  // delay(500);
}

void hallEffectInterrupt()
{
  state = !state;
  lastDeltaTime = millis() - lastTime;
  lastTime = millis();
}
