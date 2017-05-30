//#define FASTLED_FORCE_SOFTWARE_PINS
//#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_ESP8266_D1_PIN_ORDER
#define FASTLED_INTERRUPT_RETRY_COUNT 0

#include <Arduino.h>
#include <FastLED.h>
#include <Ticker.h>

#include "debug.h"
#include "network.h"
#include "ota.h"


SwarmDebug   debug;
SwarmOTA     ota;
SwarmNetwork swarm;
Ticker       ticker;

#define NUM_LEDS 240
#define NUM_STRANDS 4
CRGB       leds[NUM_LEDS*NUM_STRANDS];

#define WEMOS_BUTTON D3

#define OTA_SSID "nvtestwireless"
#define OTA_PASS ""
#define OTA_HOST "fornander.com"
#define OTA_PATH "/firmware.bin"


void DoOTA()
{
    ota.RebootUpdate(OTA_SSID, OTA_PASS, OTA_HOST, OTA_PATH);
}

void Receive(uint32_t from, String& message)
{
    //Serial.printf(NAME ": Received msg=%s\n", message.c_str());

    if (message == "ota")
        ticker.attach(5, DoOTA);
}

void setup()
{
    debug.Info("LightSwarm Node: " NAME);
    ota.TryUpdate();

    FastLED.addLeds<WS2811_PORTA, NUM_STRANDS, GRB>(leds, NUM_LEDS);
    set_max_power_in_volts_and_milliamps(5, 4000);
    memset8(leds, 0, NUM_LEDS*NUM_STRANDS*3);

    swarm.Init();
    swarm.SetReceived(&Receive);

    pinMode(WEMOS_BUTTON, INPUT);
}

void debugFunc()
{
    EVERY_N_SECONDS(2)
    {
        debug.SetLed(true);
        debug.Info(NAME ": fps=%d time=%u nodes=%u",
            LEDS.getFPS(), swarm.GetTime(), swarm.GetNodeCount());
    }
}

void animate()
{
    fill_rainbow(leds, NUM_LEDS, swarm.GetTime() / (1000*4), -1);

    CRGB* one =   leds + NUM_LEDS;
    CRGB* two =   leds + NUM_LEDS*2;
    CRGB* three = leds + NUM_LEDS*3;

    for (int i = 0; i < NUM_LEDS; i++)
    {
        one[i] =   leds[i];
        two[i] =   leds[i];
        three[i] = leds[i];
    }
}

void loop()
{
    debugFunc();
    swarm.Update();

    EVERY_N_MILLISECONDS(10)
    {
        animate();
    }

    show_at_max_brightness_for_power();

    delay(1);  // yield();
    debug.SetLed(false);


    if (!digitalRead(WEMOS_BUTTON))
    {
        while (!digitalRead(WEMOS_BUTTON))
            delay(1);
        swarm.Broadcast(String("ota"));

        ticker.attach(5, DoOTA);
    }
}
