
#include <Arduino.h>
#include "debug.h"

#define LED_BUILTIN 2

namespace LightSwarm {


Debug::Debug()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.begin(115200);
    while (!Serial) ; // Wait for serial port to be available
    Serial.println("!");
}

void Debug::SetLed(bool on)
{
    digitalWrite(LED_BUILTIN, on ? LOW : HIGH);

}

void Debug::Info(const char* format, ...)
{
    char buffer[128];
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), reinterpret_cast<const char*>(format), args);
    va_end(args);

    Serial.println(buffer);
}


Debug& Debug::GetSingleton()
{
    static Debug s_Debug;
    return s_Debug;
}

} // namespace
