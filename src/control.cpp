#include <Arduino.h>
#include <ArduinoJson.h>
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

	const int kStartAnimation = 1;
	m_CurrrentAnimation = kStartAnimation + 1;
	SelectAnimation(kStartAnimation);
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
	m_Network.Broadcast(message);
}


void Control::OnTurn(int inDelta)
{
	StaticJsonBuffer<kMaxJson> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["msg"] = "SetAnim";
	root["index"] = m_Value;

	char output[kMaxJson];
	root.prettyPrintTo(output, kMaxJson);


	INFO(" [CTRL] Turn value=%u delta=%d json=%s\n", m_Value, inDelta, output);


	SelectAnimation(m_Value);
	m_Network.Broadcast(output);
}


void Control::OnMessage(uint32_t inFromNodeID, const String& inMessage)
{
	INFO(" [CTRL] <%x>:  Received msg=%s from=%x\n", m_Network.GetNodeID(), inMessage.c_str(), inFromNodeID);

	StaticJsonBuffer<kMaxJson> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(inMessage.c_str());

	// Test if parsing succeeds.
	if (!root.success())
	{
		WARN("Not JSON");
		return;
	}

	if (!root.containsKey("msg"))
	{
		WARN("Not a message");
		return;
	}

	const char* msg = root["msg"];
	if (0 == strcmp(msg, "SetAnim"))
		SelectAnimation(root["index"]);
}


void Control::SelectAnimation(int inIndex)
{
	const int kPresentationCount = 2;
	inIndex %= kPresentationCount;

	if (inIndex == m_CurrrentAnimation)
		return;
	m_CurrrentAnimation = inIndex;


	m_Player.FreePresentations();

	char*			theSequence;
	uint32_t		theByteCount;
	CPresentation*  thePresentation;


	inIndex %= kPresentationCount;
	INFO(" [CTRL] Switching to pres=%u\n", inIndex);
	switch (inIndex)
	{
		case 1:
		{
			thePresentation  = new CPresentation();
			CPresentation::CreateSequence(9, theSequence, theByteCount);
			thePresentation->AddStrand(theSequence, theByteCount);
			m_Player.AddPresentation(thePresentation, m_Network.GetNodeOffset());

			break;
		}

		default:
		{
			CPresentation::CreateSequence(4, theSequence, theByteCount);
			thePresentation  = new CPresentation();
			thePresentation->AddStrand(theSequence, theByteCount);
			m_Player.AddPresentation(thePresentation, m_Network.GetNodeOffset());

			CPresentation::CreateSequence(3, theSequence, theByteCount);
			thePresentation = new CPresentation();
			thePresentation->AddStrand(theSequence, theByteCount);
			m_Player.AddPresentation(thePresentation, m_Network.GetNodeOffset());

			CPresentation::CreateSequence(8, theSequence, theByteCount);
			thePresentation  = new CPresentation();
			thePresentation->AddStrand(theSequence, theByteCount);
			m_Player.AddPresentation(thePresentation, m_Network.GetNodeOffset());

			break;
		}
	}
}



} // namespace
