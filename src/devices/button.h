#pragma once

namespace LightSwarm {

// The instataneous state of the button up or down
enum EButtonState
{
    State_Up = 0,
    State_Down
};

// The tracked state of the button for click management
enum EButtonClick
{
    Click_None = 0,
    Click_Single,
    Click_Long,
    // FUTURE: Click_Double,
};

// This library manages a push button control by default
// this is the pin G39 built in to the Atom-lite.
class Button
{
public:
    // Define which pins to use for the button, the default atom-lite mainbutton is G39
    Button(int pin = G39);

    // Track and return the click state of the push button
    EButtonClick Poll();

    // Return the instantaneous status of the push button with no state
    EButtonState ReadState();

private:
    const uint32_t  m_LongClickLength{1000};
    const int       m_Pin{-1};

    EButtonState    m_LastState{State_Up};
    uint32_t        m_PressTime{0};
};
}
