//
// RotaryEncoder.cpp
//

#include <Arduino.h>
#include "rotary.h"

RotaryEncoder::RotaryEncoder(OptPin gnd, OptPin vcc, OptPin sw, Pin dt, Pin clk) :
               gndPin(gnd),
               vccPin(vcc),
               swPin(sw),
               dtPin(dt),
               clkPin(clk),
               maxBias(1),
               bias(1),
               clk(false),
               dt(false) {
} // RotaryEncoder::RotaryEncoder(gnd, vcc, sw, dt, clk)

void RotaryEncoder::Begin(byte maxBias)
{
   this->maxBias = maxBias;
   if (gndPin!=NoPin)
   {
      pinMode(gndPin, OUTPUT); // Let's power the device,
      digitalWrite(gndPin, LOW);  // Power -ve with 0
   } // if
   if (vccPin!=NoPin)
   {
      pinMode(vccPin, OUTPUT); // ,with both - and +
      digitalWrite(vccPin, HIGH); // Power +ve with 1
   } // if
   if (swPin!=NoPin)
   {
      pinMode(swPin, INPUT_PULLUP); // They pushed it!
   } // if
   pinMode(dtPin,  INPUT); // One half
   pinMode(clkPin, INPUT); // The other half
} // RotaryEncoder::Begin(maxBias)

bool RotaryEncoder::Switch() const
{
   return swPin!=NoPin ?
          digitalRead(swPin)==LOW :
          false;
} // RotaryEncoder::Switch()

int RotaryEncoder::Get() {
   bool clkNew = digitalRead(clkPin);
   if (clk!=clkNew)
   {
      clk = clkNew; // Clock's changed; sample data
      bool dtNew = digitalRead(dtPin);
      if (dt!=dtNew)
      {
         dt = dtNew; // Data's changed; remember it!
         //if (clk)
         {  // Only pay attention to Clk Low-High
            int temp = bias;
            bias = maxBias;
            if (dt^clk)
            {
               return -temp;
            } // if
            else
            {
               return +temp;
            } // else
         } // if
      } // if
   } // if
   return 0;
} // RotaryEncoder::Get()

void RotaryEncoder::Rebias(int delta)
{
   if (bias<=1)
   {
       return;
   } // if
   bias -= bias>delta ? delta : bias-1;
} // RotaryEncoder::Rebias()
