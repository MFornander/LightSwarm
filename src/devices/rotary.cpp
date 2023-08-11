#include <Arduino.h>
#include "rotary.h"

namespace LightSwarm {

RotaryEncoder::RotaryEncoder()
{}

RotaryEncoder::RotaryEncoder(int dt, int clk, int sw) :
    dtPin(dt),
    clkPin(clk),
    swPin(sw)
{}

void RotaryEncoder::Begin(byte inMaxBias)
{
    // NULL mode
    if (dtPin < 0) return;

    maxBias = inMaxBias;
    pinMode(swPin, INPUT_PULLUP);
    pinMode(dtPin,  INPUT);
    pinMode(clkPin, INPUT);
}

bool RotaryEncoder::Switch() const
{
    // NULL mode
    if (dtPin < 0) return false;

    return digitalRead(swPin) == LOW;
}

int RotaryEncoder::Get()
{
    // NULL mode
    if (dtPin < 0)  return 0;

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
    // NULL mode
    if (dtPin < 0) return;

    if (bias <= 1) return;

    bias -= bias > inDelta ? inDelta : bias - 1;
}

// TODO: Change the rotary architecture to manage more internally but
// factor out only the response/control code.  Follow the pattern
// of button.h/cpp where the tracking is performed in the button
// and only the control response (network, messages, effects) are
// explicitly in the control.cpp.  Otherwise with multiple
// inoput controls the control.cpp code gets littered with
// timers and trackers (for example "OnClick" could mean any device)
// 
// 
// uint32_t        m_LastTime = 0;
// uint8_t         m_Value = 100;
// bool            m_LastSwitch = false;
// uint32_t        m_SwitchTime = 0;
//
//
// void Control::ReadEncoder()
// {
//     bool theCurrentSwitch = m_Encoder.Switch();
//     uint32_t theCurrentTime = millis();

//     if (theCurrentSwitch ^ m_LastSwitch)
//     {
//         if (theCurrentSwitch)
//             m_SwitchTime = theCurrentTime;
//         else
//             OnClick(theCurrentTime - m_SwitchTime);

//         m_LastSwitch = theCurrentSwitch;
//     }
//     else
//     {
//         int delta = m_Encoder.Get();
//         if (delta != 0)
//         {
//             m_Value += delta;
//             OnTurn(delta);
//         }

//         if (m_LastTime != theCurrentTime)
//         {
//             m_LastTime = theCurrentTime;
//             m_Encoder.Rebias();
//         }
//     }
// }

// void Control::OnClick(uint32_t inMillisDown)
// {
//     const bool longClick = inMillisDown > 1000;

//     INFO(" [CTRL] Click duration=%u long=%s\n", inMillisDown, longClick ? "YES" : "no");

//     if (longClick)
//     {
//         StaticJsonDocument<kMaxJson> root;
//         root["msg"] = "SetAnim";
//         root["idx"] = m_Value;
//         root["spd"] = m_Speed;

//         char output[kMaxJson];
//         serializeJsonPretty(root, output, kMaxJson);

//         m_Network.Broadcast(output);
//     }
//     else
//     {
//         SelectSpeed(++m_Speed);
//     }
// }

// void Control::OnTurn(int inDelta)
// {
//     INFO(" [CTRL] Turn value=%u delta=%d\n", m_Value, inDelta);
//     SelectAnimation(m_Value);
// }

}
