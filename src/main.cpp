//#include <RH_RF95.h>
//#define FASTLED_FORCE_SOFTWARE_PINS
#include <Arduino.h>
#define FASTLED_ESP8266_D1_PIN_ORDER
//#define FASTLED_INTERRUPT_RETRY_COUNT 0
//#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <my9291.h>

#include "swarm.h"

#define LED_BUILTIN 2
#define LONEWOLF_DATA_PIN 4
#define MY9291_DI_PIN   13
#define MY9291_DCKI_PIN 15

#define NUM_LEDS 240
#define NUM_STRANDS 4



CRGB leds[NUM_LEDS*NUM_STRANDS];
swarm  swarm;
my9291 _my9291 = my9291(MY9291_DI_PIN, MY9291_DCKI_PIN, MY9291_COMMAND_DEFAULT);

void setup()
{
    //WiFi.setSleepMode(WIFI_NONE_SLEEP);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // Ensure serial flash is not interfering with radio communication on SPI bus
    Serial.begin(115200);
    while (!Serial) ; // Wait for serial port to be available

#if TYPE == 1
    FastLED.addLeds<WS2811_PORTA, NUM_STRANDS, GRB>(leds, NUM_LEDS);
#else
    FastLED.addLeds<WS2812, LONEWOLF_DATA_PIN, GRB>(leds, NUM_LEDS);
#endif

    //FastLED.addLeds<WS2813, DATA_PIN, GRB>(leds, NUM_LEDS);
    set_max_power_in_volts_and_milliamps(5, 4000);
}

void debug()
{
    EVERY_N_SECONDS(1)
    {
        Serial.printf(NAME ": fps=%d t=%u\n", LEDS.getFPS(), swarm.getNodeTime());
        static bool debug_blink = false;
        digitalWrite(LED_BUILTIN, debug_blink = !debug_blink ? HIGH : LOW);
    }
}

void animate()
{
    fill_rainbow(leds, NUM_LEDS*NUM_STRANDS, swarm.getNodeTime() / (1000*10), 10);
}

void loop()
{
    debug();
    swarm.update();

    EVERY_N_MILLISECONDS(10)
    {
        animate();
    }

    show_at_max_brightness_for_power();
    delay(1);

#if TYPE == 3
    _my9291.setColor((my9291_color_t) { leds[0].red, leds[0].green, leds[0].blue, 0 });
    _my9291.setState(true);
#endif
}
