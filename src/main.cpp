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
RotaryEncoder   encoder(D1, D2, D3);
Control         control(network, encoder, player);

void report()
{
	debug.SetLed( network.GetNodeCount() == 0 );
	INFO("%s: id=<%x> fps=%d time=%x nodes=%u stab=%d frame=%u retry=%u\n",
		Version::BUILD, network.GetNodeID(), LEDS.getFPS(), network.GetTime(), network.GetNodeCount() + 1,
		network.GetStability(), _frame_cnt, _retry_cnt);
}

void setup()
{
	network.Init();
}

void loop()
{
	EVERY_N_SECONDS(10) { report(); }

	control.Update();
}
