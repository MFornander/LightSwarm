#include <Arduino.h>
#include "control.h"
#include "debug.h"

namespace LightSwarm {


Control::Control()
{
}

void Control::ProcessMessage(const String& message)
{
    INFO("Control: %s", message.c_str());

}

} // namespace
