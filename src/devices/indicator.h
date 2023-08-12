#pragma once

#include <FastLED.h>

namespace LightSwarm {

// This library manages a single rgb led
// by default this is the indicator light on atom-lite
// Due to compile time restrictions this must be template
template <int pin = G27>
class Indicator
{
private:
    CRGB        m_Color{0,0,0};

public:
    Indicator() { FastLED.addLeds<NEOPIXEL, pin>(&m_Color, 1); }
    void    Set(CRGB const& color) { m_Color = color; }
};
}
