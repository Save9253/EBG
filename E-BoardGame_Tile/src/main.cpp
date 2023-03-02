#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN 10

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 1

char pinsOut[] = {6, 5, 4, 2, 1, 0}; // Ports check outputs bits 0-4
char pinsIn[] = {3};                 // Port1 return
char portValue[] = {0};
char colors[] = {
    {0, 0, 50},  // Blue
    {0, 50, 50}, // Cyan
    {0, 50, 0},  // Green
    {50, 50, 0}, // Yellow
    {50, 0, 0},  // Red
    {50, 0, 50}, // Purple
}

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products

void setup()
{
  strip.begin();
  Wire.begin(4); // join i2c bus with address #4
  Wire.onReceive(reciveEvent);
  Wire.onRequest(requestEvent); // register event
  for (char i = 0; i < 4; i++)
  {
    pinMode(pinsOut[i], OUTPUT);
  }
  for (char i = 0; i < 2; i++)
  {
    pinMode(pinsIn[i], INPUT);
  }
}

void loop()
{
  delay(100);
}

char command = 0b0;

void reciveEvent(int howMany)
{
  while (Wire.available())
  {
    command = Wire.read(); // receive byte as a character
  }
}

void requestEvent()
{
  if (command == 0b101)
  {
    for (char i = 0; i < 2; i++)
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
    ShiftColor();
  }
}
void portCheck()
{
  portValue[0] = 0;
  for (byte x = 0; x < 4; x++)
  {
    digitalWrite(pinsOut[x], HIGH);
    // Serial.print("Port check # ");
    // Serial.print(i);
    // Serial.print(" Result: ");
    for (char y = 0; y < 2; y++)
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

char currentColorIndex = 0;
void ShiftColor()
{
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