#include <Arduino.h>
#include <Servo.h>

// put function declarations here:
int myFunction(int, int);

Servo testServo;
double i = 0;
void setup()
{
  // put your setup code here, to run once:
  testServo.attach(3);
}

void loop()
{
  // put your main code here, to run repeatedly:
  int angle = 90 * sin(i) + 90;
  testServo.write(angle);

  i += 0.1;

  _delay_ms(25);
}

// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
}