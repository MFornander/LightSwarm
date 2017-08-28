#include <Arduino.h>
#include "control.h"
#include "debug.h"
#include "network.h"
#include "rotary.h"
#include "vunsq.h"

namespace LightSwarm {


Control::Control(Network& inNetwork, RotaryEncoder& inEncoder, CVunsq& inPlayer) :
	m_Network(inNetwork),
	m_Encoder(inEncoder),
	m_Player(inPlayer)
{
	using namespace std::placeholders;

	m_Network.SetReceived( std::bind(&Control::OnMessage, this, _1, _2) );
	m_Encoder.Begin(5);

	m_SwitchTime = millis();
}


void Control::Update()
{
	ReadEncoder();

	m_Network.Update();
	m_Player.Step(m_Network.GetTime() / 10000);
}


void Control::ReadEncoder()
{
	bool theCurrentSwitch = m_Encoder.Switch();
	uint32_t theCurrentTime = millis();

	if (theCurrentSwitch ^ m_LastSwitch)
	{
		if (theCurrentSwitch)
			m_SwitchTime = theCurrentTime;
		else
			OnClick(theCurrentTime - m_SwitchTime);

		m_LastSwitch = theCurrentSwitch;
	}
	else
	{
		int delta = m_Encoder.Get();
		if (delta != 0)
		{
			m_Value += delta;
			OnTurn(delta);
		}

		if (m_LastTime != theCurrentTime)
		{
			m_LastTime = theCurrentTime;
			m_Encoder.Rebias();
		}
	}
}


void Control::OnClick(uint32_t inMillisDown)
{
	INFO(" [CTRL] Click duration=%u long=%s\n", inMillisDown, inMillisDown > 1000 ? "YES" : "no");
	String message = "My value is " + String(m_Value, DEC);
	Broadcast(message);
}


void Control::OnTurn(int inDelta)
{
	INFO(" [CTRL] Turn value=%u delta=%d\n", m_Value, inDelta);
}


void Control::OnMessage(uint32_t inFromNodeID, const String& inMessage)
{
	INFO(" [CTRL] <%x>:  Received msg=%s from=%x\n", m_Network.GetNodeID(), inMessage.c_str(), inFromNodeID);
}


void Control::Broadcast(const String& inMessage)
{
	//INFO(" [CTRL] <%x>:  Sending=%s\n", inMessage.c_str());
	m_Network.Broadcast(inMessage);
}

} // namespace
