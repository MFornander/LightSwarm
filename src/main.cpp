#define FASTLED_DEBUG_COUNT_FRAME_RETRIES
#include <Arduino.h>
#include <FastLED.h>
#include <Ticker.h>

#include "debug.h"
#include "network.h"
#include "presentation.h"
#include "version.h"
#include "vunsq.h"

using namespace LightSwarm;

Debug           debug;
Network         network;
CVunsq          player;
CPresentation   backPres( "/effect3only.vunsq" );
CPresentation   frontPres( "/effect3only.vunsq" );

void report()
{
    debug.SetLed( network.GetNodeCount() == 0 );
    INFO("%s: id=%x fps=%d time=%x nodes=%u stab=%d frame=%u retry=%u\n",Version::BUILD, network.GetNodeID(), LEDS.getFPS(), network.GetTime(), network.GetNodeCount() + 1, network.GetStability(), _frame_cnt, _retry_cnt);
}

void setup()
{
    network.Init();
    player.AddPresentation(&backPres, network.GetNodeOffset(), 1);
    player.AddPresentation(&frontPres, network.GetNodeOffset(), 0);
}

void loop()
{
    EVERY_N_SECONDS(2) { report(); }

    network.Update();
	player.Step(network.GetTime() / 10000);
}
