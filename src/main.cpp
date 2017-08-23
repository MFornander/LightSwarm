#include <Arduino.h>
#include <Ticker.h>

#include "hal.h"
#include "debug.h"
#include "network.h"
#include "presentation.h"
#include "version.h"
#include "vunsq.h"

#include "rotary.h"
const byte LED = LED_BUILTIN;
const unsigned baud = 115200;
const RotaryEncoder::OptPin GND = -1; // Use GND pin
const RotaryEncoder::OptPin VCC = -1; // Use V+ pin
const RotaryEncoder::OptPin SW  =  D3; // Switch
const RotaryEncoder::Pin    DT  =  D2; // Data
const RotaryEncoder::Pin    CLK =  D1; // Clock

// Tell library which pin does what. Note Optional Pins can be -1
RotaryEncoder encoder(GND, VCC, SW, DT, CLK);

using namespace LightSwarm;

Debug           debug;
Network         network;
CVunsq          player;
CPresentation   backPres;
CPresentation   frontPres;
CPresentation   testPres;

void report()
{
    debug.SetLed( network.GetNodeCount() == 0 );
    INFO("%s: id=<%x> fps=%d time=%x nodes=%u stab=%d frame=%u retry=%u\n",
        Version::BUILD, network.GetNodeID(), LEDS.getFPS(), network.GetTime(), network.GetNodeCount() + 1,
        network.GetStability(), _frame_cnt, _retry_cnt);
}

void setup()
{
    encoder.Begin(10); // Start, with a re-bias of 10
    
    
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

bool ledON = false;  // Current LED state
byte repeat = 0x80;  // How quickly to restart, in ms
byte rotcount = repeat; // Count down 'til next time!
unsigned last = 0;   // Last time millis() was recorded

void loop()
{
    // Have switch override pulsing - turn it hard ON
    if (encoder.Switch()) { // Hard override!
        ledON = false; // Pretend LED is now OFF
        digitalWrite(LED, LOW);
    } // if
    else {
        int value = encoder.Get(); // Returns positive for clockwise turn
        repeat += -value;   // We want to INCREASE time to slow down blinking
        if (value!=0) {     // Did it change?
            Serial.print(value);    // Print latest value
            Serial.print('=');
            Serial.println(repeat); // Print new result
        } // if
    } // else

    // Is it time to blink the LED again?
    unsigned latest = millis();
    if (last!=latest) {
        last = latest;
        encoder.Rebias();    // Re-bias encoder, to decelerate rotation
        if (--rotcount==0) {    // Reached zero yet?
            ledON = !ledON;  // Yes! Toggle LED
            digitalWrite(LED, ledON ? HIGH : LOW);
            rotcount = repeat;  // Count for next time
       } // if
    } // if

    return;
    
    EVERY_N_SECONDS(2) { report(); }

    network.Update();
	player.Step(network.GetTime() / 10000);
}
