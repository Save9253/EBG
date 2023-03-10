#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN 10

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 2

byte pinsOut[] = {6, 5, 4, 2, 1, 0}; // Ports check outputs bits
byte pinsIn[] = {3, 9};              // Port1 return
byte portValue[] = {0, 0};
byte colors[][3] = {
    {0, 0, 50},  // Blue
    {0, 50, 50}, // Cyan
    {0, 50, 0},  // Green
    {50, 50, 0}, // Yellow
    {50, 0, 0},  // Red
    {50, 0, 50}, // Purple
};

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:

byte command = 0b0;

void reciveEvent(int howMany)
{
  while (Wire.available())
  {
    command = Wire.read(); // receive byte as a byteacter
  }
}

void portCheck()
{
  portValue[0] = 0;
  for (byte x = 0; x < 6; x++)
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

byte currentColorIndex = 0;
void ShiftColor()
{
  strip.setPixelColor(1, 0, 0, 0);                                                                                  // Pixel number, Red (0-255), Green (0-255), Blue (0-255)
  strip.setPixelColor(0, colors[currentColorIndex][0], colors[currentColorIndex][1], colors[currentColorIndex][2]); // Pixel number, Red (0-255), Green (0-255), Blue (0-255)
  strip.show();
  if (currentColorIndex < 5) // everytime the colorshift comand will be called the color will shift to the next one
  {
    currentColorIndex++;
  }
  else // after the fift color it will reset back to 0
  {
    currentColorIndex = 0;
  }
}

void requestEvent()
{
  if (command == 0b101)
  {
    // for (byte i = 0; i < 2; i++)
    //{
    Serial.print("Check\n");
    portCheck();
    Wire.write(portValue[0]);
    //}
  }
  if (command == 0b110)
  {
    ShiftColor();
  }
}

void setup()
{
  strip.begin();
  strip.setPixelColor(1, 10, 10, 0); // Pixel number, Red (0-255), Green (0-255), Blue (0-255)
  strip.show();
  Wire.begin(4); // join i2c bus with address #4
  Wire.onReceive(reciveEvent);
  Wire.onRequest(requestEvent); // register event
  for (byte i = 0; i < 6; i++)
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