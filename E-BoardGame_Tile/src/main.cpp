#include <Arduino.h>
#include <Wire.h>

byte pinsOut[] = {6, 5, 4, 2, 1, 0}; // Ports check outputs bits 0-4
byte pinsIn[] = {3};                 // Port1 return
byte NeoPixel = 10;
byte portValue[] = {0};

void setup()
{
  Wire.begin(4); // join i2c bus with address #4
  Wire.onReceive(reciveEvent);
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);           // start serial for output
  pinMode(NeoPixel, OUTPUT);
  for (byte i = 0; i < 4; i++)
  {
    pinMode(pinsOut[i], OUTPUT);
  }
  for (byte i = 0; i < 2; i++)
  {
    pinMode(pinsIn[i], INPUT);
  }
}

void loop()
{
  delay(100);
}

byte command = 0b0;

void reciveEvent(int howMany)
{
  Serial.print("RequestEvent\n");
  Serial.print("\n");
  while (Wire.available())
  {
    command = Wire.read(); // receive byte as a character
  }
}

void requestEvent()
{
  Serial.print("RequestEvent\n");
  if (command == 0b101)
  {
    for (byte i = 0; i < 2; i++)
    {
      // Serial.print("Check\n");
      // Serial.print(portValue);
      // Serial.print("\n");
      portCheck();
      Wire.write(portValue[i]);
    }
  }
  else if (command == 0b110)
  {
    blinkF();
    Serial.print("Blink\n");
  }
}
void portCheck()
{
  portValue[0] = 0;
  portValue[1] = 0;
  for (byte x = 0; x < 4; x++)
  {
    digitalWrite(pinsOut[x], HIGH);
    // Serial.print("Port check # ");
    // Serial.print(i);
    // Serial.print(" Result: ");
    for (byte y = 0; y < 2; y++)
    {
      portValue[y] |= digitalRead(pinsIn[y]) << x;
    }
    // Serial.print(digitalRead(port1_R),BIN);
    // Serial.print(" _ ");
    // Serial.print(portReturn,BIN);
    // Serial.print("\n");
    digitalWrite(pinsOut[x], LOW);
  };
}

void blinkF()
{
  digitalWrite(NeoPixel, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(500);                   // wait for a second
  digitalWrite(NeoPixel, LOW);  // turn the LED off by making the voltage LOW
  delay(500);
  digitalWrite(NeoPixel, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(500);                   // wait for a second
  digitalWrite(NeoPixel, LOW);  // turn the LED off by making the voltage LOW
}