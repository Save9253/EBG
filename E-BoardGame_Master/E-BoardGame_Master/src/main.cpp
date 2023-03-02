#include <Arduino.h>
#include <Wire.h>

void checkPort()
{
  Wire.beginTransmission(4); // transmit to device #4
  Serial.print("Requesting Ports\n");
  Wire.write(0b101);      // sends one byte
  Wire.endTransmission(); // stop transmitting
  Wire.requestFrom(4, 2);
  while (Wire.available())
  {
    byte c = Wire.read(); // receive byte as a character
    Serial.print(c, BIN);
    Serial.print("\n");
  }
}

void shiftColor()
{
  Wire.beginTransmission(4); // transmit to device #4
  Serial.print("Requesting color shift\n");
  Wire.write(0b110);      // sends one byte
  Wire.endTransmission(); // stop transmitting
}

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  // Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

void loop()
{
  for (int i = 0; i < 2; i++)
  {
    if (i == 0)
    {
      checkPort();
    }
    else if (i == 1)
    {
      shiftColor();
    }
    delay(500);
  }
  delay(3000);
}
