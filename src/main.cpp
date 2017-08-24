#include <Arduino.h>
#include <Ticker.h>

#include "hal.h"
#include "debug.h"
#include "control.h"
#include "network.h"
#include "presentation.h"
#include "rotary.h"
#include "version.h"
#include "vunsq.h"

using namespace LightSwarm;


Debug           debug;
Network         network;
CVunsq          player;
CPresentation   backPres;
CPresentation   frontPres;
CPresentation   testPres;
RotaryEncoder   encoder(D1, D2, D3);
Control         control(network, encoder);

void report()
{
    debug.SetLed( network.GetNodeCount() == 0 );
    INFO("%s: id=<%x> fps=%d time=%x nodes=%u stab=%d frame=%u retry=%u\n",
        Version::BUILD, network.GetNodeID(), LEDS.getFPS(), network.GetTime(), network.GetNodeCount() + 1,
        network.GetStability(), _frame_cnt, _retry_cnt);
}

void setup()
{
    char* theSequence;
    uint32_t theByteCount;

    network.Init();

    CPresentation::CreateSequence(4, theSequence, theByteCount);
    backPres.AddStrand(theSequence, theByteCount);
    player.AddPresentation(&backPres, network.GetNodeOffset());

    CPresentation::CreateSequence(3, theSequence, theByteCount);
    frontPres.AddStrand(theSequence, theByteCount);
    player.AddPresentation(&frontPres, network.GetNodeOffset());

    CPresentation::CreateSequence(8, theSequence, theByteCount);
    testPres.AddStrand(theSequence, theByteCount);
    player.AddPresentation(&testPres, network.GetNodeOffset());
}

void loop()
{
    EVERY_N_SECONDS(2) { report(); }

    network.Update();
    control.Update();
    
	player.Step(network.GetTime() / 10000);
}
