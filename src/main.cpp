//#define FASTLED_FORCE_SOFTWARE_PINS
//#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_ESP8266_D1_PIN_ORDER
#define FASTLED_INTERRUPT_RETRY_COUNT 0

#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>

#include "debug.h"
#include "swarm.h"


#define NUM_LEDS 240
#define NUM_STRANDS 4

CRGB       leds[NUM_LEDS*NUM_STRANDS];
SwarmDebug debug;
Swarm      swarm;


void setup()
{
    FastLED.addLeds<WS2811_PORTA, NUM_STRANDS, GRB>(leds, NUM_LEDS);
    set_max_power_in_volts_and_milliamps(5, 4000);

    memset8(leds, 0, NUM_LEDS*NUM_STRANDS*3);
}

void debugFunc()
{
    EVERY_N_SECONDS(1)
    {
        debug.SetLed(true);
        Serial.printf(NAME ": fps=%d t=%u\n", LEDS.getFPS(), swarm.getNodeTime());
    }
}

void animate()
{
    fill_rainbow(leds, NUM_LEDS, swarm.getNodeTime() / (1000*10), 10);

    CRGB* one =   leds + NUM_LEDS;
    CRGB* two =   leds + NUM_LEDS*2;
    CRGB* three = leds + NUM_LEDS*3;

    for (int i = 0; i < NUM_STRANDS; i++)
    {
        one[i].red =    leds[i].red;
        two[i].green =  leds[i].green;
        three[i].blue = leds[i].blue;
    }
}

void loop()
{
    debugFunc();
    swarm.update();

    EVERY_N_MILLISECONDS(10)
    {
        animate();
    }

    show_at_max_brightness_for_power();
    delay(1);


    debug.SetLed(false);
}
