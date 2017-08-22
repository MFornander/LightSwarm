
#include <Arduino.h>
#include "debug.h"

#define LED_BUILTIN 2
#define MESSAGE_SIZE 512

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
    char buffer[MESSAGE_SIZE];
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Serial.print(buffer);
}

void Debug::Warn(const char* format, ...)
{
    char buffer[MESSAGE_SIZE];
    buffer[0] = 'W';
    buffer[1] = ':';
    va_list args;

    va_start(args, format);
    vsnprintf(buffer+2, sizeof(buffer)-2, format, args);
    va_end(args);

    Serial.print(buffer);
}

void Debug::Error(const char* format, ...)
{
    char buffer[MESSAGE_SIZE];
    buffer[0] = 'E';
    buffer[1] = ':';
    va_list args;

    va_start(args, format);
    vsnprintf(buffer+2, sizeof(buffer)-2, format, args);
    va_end(args);

    Serial.print(buffer);
}


Debug& Debug::GetSingleton()
{
    static Debug s_Debug;
    return s_Debug;
}

} // namespace
