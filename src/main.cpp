//#include <RH_RF95.h>
#define FASTLED_FORCE_SOFTWARE_PINS
#include <FastLED.h>
#include <Arduino.h>
#define LED_BUILTIN 2

#define NUM_LEDS 240
#define DATA_PIN 4
CRGB leds[NUM_LEDS];

//RH_RF95 rf95(8, 3); // Rocket Scream Mini Ultra Pro with the RFM95W
//#define Serial SerialUSB


unsigned long oldTime = millis();
long oldFrames = 0;
long frames = 0;

void blink(int count = 1, int onTime = 100, int offTime = 900)
{
  for (int i=0; i<count; i++)
  {
    digitalWrite(LED_BUILTIN, LOW);
    for (int ledID=0; ledID < NUM_LEDS; ledID++)
      leds[ledID] = CRGB::DeepPink;
    FastLED.show();
    //delay(onTime);
    digitalWrite(LED_BUILTIN, HIGH);
    for (int ledID=0; ledID < NUM_LEDS; ledID++)
      leds[ledID] = CRGB::Black;
    FastLED.show();
    //delay(offTime);
  }
}


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Ensure serial flash is not interfering with radio communication on SPI bus
  Serial.begin(115200);
  while (!Serial) ; // Wait for serial port to be available

  FastLED.addLeds<WS2813, DATA_PIN, GRB>(leds, NUM_LEDS);
}

void loop()
{
    unsigned long currentTime = millis();
    if (currentTime > oldTime + 1000)
    {
        Serial.println(frames - oldFrames);
        oldFrames = frames;
        oldTime = currentTime;
    }



    if ((frames % 10) == 0)
    {
      digitalWrite(LED_BUILTIN, LOW);
      //for (int ledID=0; ledID < NUM_LEDS; ledID++)
      //  leds[ledID] = CRGB::DeepPink;
    }
    else
    {
      digitalWrite(LED_BUILTIN, HIGH);
      //for (int ledID=0; ledID < NUM_LEDS; ledID++)
      //  leds[ledID] = CRGB::Black;
    }

    fill_rainbow(leds, NUM_LEDS, frames, 1);

    delay(1);
    FastLED.show();
    frames++;
}
