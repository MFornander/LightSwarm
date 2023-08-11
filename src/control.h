#pragma once

#include "devices/rotary.h"
#include "devices/button.h"
#include "devices/indicator.h"

namespace LightSwarm {

class Network;
class CVunsq;

/**
 * Core class that ties inputs and events to method calls.
 *
 * Inputs can be networking events, GPIO changes, etc */
class Control final
{
public:
    Control(Network& inNetwork, CVunsq& inPlayer);

    void Update();

    void OnClick(uint32_t inMillisDown);
    void OnTurn(int inDelta);
    void OnMessage(uint32_t inFromNodeID, const String& inMessage);

protected:
    void SelectAnimation(int inIndex);
    void SelectSpeed(int inSpeed);

private:
    Network&        m_Network;
    CVunsq&         m_Player;
    Button          m_MainButton;
    Indicator<>     m_Indicator;

    int             m_CurrrentAnimation;
    int             m_Speed = 0;

    static const size_t kMaxJson = 100;
};
}
