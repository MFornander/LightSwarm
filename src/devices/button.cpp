#include <Arduino.h>
#include "button.h"
#include "debug.h"

namespace LightSwarm {

Button::Button(int pin) :
    m_Pin(pin)
{
    // Initialize the pushbutton pin as an input and set the state value
    pinMode(m_Pin, INPUT_PULLUP);  // INPUT
    m_LastState = ReadState();
}

EButtonClick Button::Poll()
{
    // Read the state and time of the pushbutton value:
    EButtonState currentState = ReadState();
    uint32_t currentTime = millis();
    EButtonClick clickResult = Click_None;

    if (currentState != m_LastState)
    {
        // If the button was released resulting in a click
        if (currentState == State_Up)
        {
            // Short press
            if (currentTime - m_PressTime < m_LongClickLength)
                clickResult = Click_Single;
            // Long press
            else
                clickResult = Click_Long;
        }

        m_LastState = currentState;
        m_PressTime = currentTime;
    }

    return clickResult;
}

EButtonState Button::ReadState()
{
    // read the state of the pushbutton value:
    return digitalRead(m_Pin) == HIGH ? State_Up : State_Down;
}
}
