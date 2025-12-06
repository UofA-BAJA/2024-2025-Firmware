#include <Arduino.h>

// put function declarations here:
void checkBatt();

const float referenceVolts = 5.0;
const float R1 = 10000.0;
const float R2 = 18800.0;
const int batteryPin = A0;
float battV;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // pinMode(A0, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:


  int val = analogRead(batteryPin);
  battV = (val * (5.0/1024.0)) * ((R1 + R2) / R1);
  float voltageIn = (val * (5.0/1024.0)); 
  Serial.print(val);
  Serial.print(" / ");
  Serial.print(voltageIn);
  Serial.print(": ");
  Serial.print(battV);
  Serial.println("V");
  delay(500);
}

