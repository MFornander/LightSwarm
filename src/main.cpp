#include <Arduino.h>

#include "hal.h"
#include "debug.h"
#include "control.h"
#include "network.h"
#include "presentation.h"
#include "rotary.h"
#include "version.h"
#include "vunsq.h"

using namespace LightSwarm;

Network         network;
CHAL			hal(network.GetNodeID());
CVunsq          player(hal);
RotaryEncoder   encoder(1, 2, 3);
Control         control(network, encoder, player);

void report()
{
	Debug::GetSingleton().SetLed( network.GetNodeCount() == 0 );
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
