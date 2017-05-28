
#include <Arduino.h>
#include "debug.h"

#define LED_BUILTIN 2

SwarmDebug::SwarmDebug()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.begin(115200);
    while (!Serial) ; // Wait for serial port to be available
}

void SwarmDebug::SetLed(bool on)
{
    digitalWrite(LED_BUILTIN, on ? LOW : HIGH);

}
