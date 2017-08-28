#ifndef __SWARM_CONTROL_H__
#define __SWARM_CONTROL_H__

namespace LightSwarm {

class Network;
class RotaryEncoder;

/**
 * Core class that ties inputs and events to method calls.
 *
 * Inputs can be networking events, GPIO changes, etc */
class Control final
{
public:
	Control(Network& inNetwork, RotaryEncoder& encoder);

	void Update();

	void OnClick(uint32_t inMillisDown);
	void OnTurn(int inDelta);
	void OnMessage(uint32_t inFromNodeID, const String& inMessage);

protected:
	void Broadcast(const String& inMessage);

	private:
	Network& m_Network;
	RotaryEncoder& m_Encoder;

	uint32_t    m_LastTime = 0;
	uint8_t     m_Value = 100;
	bool        m_LastSwitch = false;
	uint32_t    m_SwitchTime = 0;
};
} // namespace

#endif
