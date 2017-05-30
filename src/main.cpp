//#define FASTLED_FORCE_SOFTWARE_PINS
//#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_ESP8266_D1_PIN_ORDER
#define FASTLED_INTERRUPT_RETRY_COUNT 0

#include <Arduino.h>
#include <FastLED.h>
#include <Ticker.h>

#include "control.h"
#include "debug.h"
#include "network.h"

#include "ota.pass"
#include "version.h"

using namespace LightSwarm;

Control control;
Debug   debug;
Network network;
OTA     ota;
Ticker  ticker;

#define OTA_SSID "nvtestwireless"
#include "ota.pass"
#define OTA_HOST "fornander.com"
#define OTA_PATH "/firmware.bin"


#define NUM_LEDS 240
#define NUM_STRANDS 4
CRGB       leds[NUM_LEDS*NUM_STRANDS];

#define WEMOS_BUTTON D3


void DoOTA()
{
    ota.RebootUpdate(OTA_SSID, OTA_PASS, OTA_HOST, OTA_PATH);
}


void Receive(uint32_t from, const String& message)
{
    if (from == network.GetNodeID())
        return;

    INFO("[NET] Received from=%x msg=%s\n", from, message.c_str());

    if (message == "ota")
        ticker.attach(5, DoOTA);
}


void debugFunc()
{
    EVERY_N_SECONDS(2)
    {
        debug.SetLed(true);
        INFO("%s: id=%x fps=%d time=%x nodes=%u\n",
            Version::BUILD,
            network.GetNodeID(),
            LEDS.getFPS(),
            network.GetTime(),
            network.GetNodeCount() + 1);
    }
}


void animate()
{
    fill_rainbow(leds, NUM_LEDS, network.GetTime() / (1000*4), -1);

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



void setup()
{
    INFO("LightSwarm %s:%x\n", Version::BUILD, network.GetNodeID());
    ota.TryUpdate();



    network.Init();
    network.SetReceived(Receive);

    // TODO(mf): Remove and refactor this to a dedicated animation class
    FastLED.addLeds<WS2811_PORTA, NUM_STRANDS, GRB>(leds, NUM_LEDS);
    set_max_power_in_volts_and_milliamps(5, 4000);
    memset8(leds, 0, NUM_LEDS*NUM_STRANDS*3);

    // TODO(mf) Remove debug OTA hack
    pinMode(WEMOS_BUTTON, INPUT);
}


void loop()
{
    debugFunc();
    network.Update();

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
        network.Broadcast(String("ota"));

        ticker.attach(5, DoOTA);
    }
}
