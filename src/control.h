#pragma once

namespace LightSwarm {

class Network;
class RotaryEncoder;
class CVunsq;

/**
 * Core class that ties inputs and events to method calls.
 *
 * Inputs can be networking events, GPIO changes, etc */
class Control final
{
public:
    Control(Network& inNetwork, RotaryEncoder& inEncoder, CVunsq& inPlayer);

    void Update();

    void OnClick(uint32_t inMillisDown);
    void OnTurn(int inDelta);
    void OnMessage(uint32_t inFromNodeID, const String& inMessage);

protected:
    void ReadEncoder();
    void SelectAnimation(int inIndex);
    void SelectSpeed(int inSpeed);

private:
    Network&        m_Network;
    RotaryEncoder&  m_Encoder;
    CVunsq&         m_Player;

    uint32_t        m_LastTime = 0;
    uint8_t         m_Value = 100;
    bool            m_LastSwitch = false;
    uint32_t        m_SwitchTime = 0;
    int             m_CurrrentAnimation;
    int             m_Speed = 0;

    static const size_t kMaxJson = 100;
};
}
