#include <Arduino.h>

#include "debug.h"
#include "vunsq.h"
#include "presentation.h"

LightSwarm::Debug  debug;
CVunsq          thePlayer;
CPresentation   thePresentation( "/effect3only.vunsq" );


void setup()
{
    thePlayer.SetPresentation(&thePresentation);
}

void loop()
{
	thePlayer.Step();
    delay(1);
}
