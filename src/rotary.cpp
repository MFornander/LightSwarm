#include <Arduino.h>
#include "rotary.h"

namespace LightSwarm {

RotaryEncoder::RotaryEncoder(int dt, int clk, int sw) :
    dtPin(dt),
    clkPin(clk),
    swPin(sw)
{}

void RotaryEncoder::Begin(byte inMaxBias)
{
    maxBias = inMaxBias;
    pinMode(swPin, INPUT_PULLUP);
    pinMode(dtPin,  INPUT);
    pinMode(clkPin, INPUT);
}

bool RotaryEncoder::Switch() const
{
    return digitalRead(swPin) == LOW;
}

int RotaryEncoder::Get()
{
    bool clkNew = digitalRead(clkPin);
    if (clk != clkNew)
    {
        clk = clkNew; // Clock's changed; sample data
        bool dtNew = digitalRead(dtPin);
        if (dt != dtNew)
        {
            dt = dtNew; // Data's changed; remember it
            if (clk)
            {
                int temp = bias;
                bias = maxBias;
                if (dt)
                    return temp;
                else
                    return -temp;
            }
        }
    }
    return 0;
}

void RotaryEncoder::Rebias(int inDelta)
{
    if (bias <= 1)
        return;

    bias -= bias > inDelta ? inDelta : bias - 1;
}
}
