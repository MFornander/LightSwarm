//#include <RH_RF95.h>
//#define FASTLED_FORCE_SOFTWARE_PINS
#include <Arduino.h>
#define FASTLED_ESP8266_D1_PIN_ORDER
//#define FASTLED_INTERRUPT_RETRY_COUNT 0
//#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define LED_BUILTIN 2

#define NUM_LEDS 240
#define DATA_PIN 4
CRGB leds[NUM_LEDS];

WiFiUDP Udp;



void setup()
{
    //WiFi.setSleepMode(WIFI_NONE_SLEEP);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // Ensure serial flash is not interfering with radio communication on SPI bus
    Serial.begin(115200);
    while (!Serial) ; // Wait for serial port to be available

    FastLED.addLeds<WS2811_PORTA, 1, GRB>(leds, NUM_LEDS);
    //FastLED.addLeds<WS2813, DATA_PIN, GRB>(leds, NUM_LEDS);
    set_max_power_in_volts_and_milliamps(5, 4000);

    // WiFi
    WiFi.begin("nvtestwireless", "Sp33doflight");

    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
}

void debug()
{
    EVERY_N_MILLISECONDS(500)
    {
        Serial.println(LEDS.getFPS());
        static bool debug_blink = false;
        digitalWrite(LED_BUILTIN, debug_blink = !debug_blink ? HIGH : LOW);
    }
}

void animate( unsigned long anim_time )
{
    fill_rainbow(leds, NUM_LEDS, anim_time / 3, 5);
}

void loop()
{
    debug();

    EVERY_N_MILLISECONDS(10)
    {
        animate( millis() );
    }

    show_at_max_brightness_for_power();
    delay(1);
}






/*
    if ((frames % 2) == 0)
    {
      digitalWrite(LED_BUILTIN, LOW);
      for (int ledID=0; ledID < NUM_LEDS; ledID++)
        leds[ledID] = CRGB::DeepPink;
    }
    else
    {
      digitalWrite(LED_BUILTIN, HIGH);
      for (int ledID=0; ledID < NUM_LEDS; ledID++)
        leds[ledID] = CRGB::Black;
    }*/
