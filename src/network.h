#pragma once
#include <painlessMesh.h>

namespace LightSwarm {

class Network final
{
public:
	Network();
	~Network();

	void Init();
	void Update();
	uint32_t GetTime();
	uint32_t GetNodeID();
	int32_t  GetNodeOffset();
	uint32_t GetNodeCount();
	uint32_t GetStability();

	using ReceivedCallbackT = std::function<void(uint32_t from, const String& msg)>;
	void Broadcast(const String& msg);
	void SetReceived(ReceivedCallbackT callback);

private:
	// Callbacks for mesh control
	void ReceivedCallback(uint32_t from, String& msg);
	void NewConnectionCallback(uint32_t nodeId);
	void ChangedConnectionCallback();
	void NodeTimeAdjustedCallback(int32_t offset);
	void DelayReceivedCallback(uint32_t from, int32_t delay);

	painlessMesh          m_mesh;
	bool                  m_calcDelay = false;
	std::list<uint32_t>	  m_nodes;
	uint32_t              m_sendMessageTime = 0;
	uint32_t              m_helloCounter = 0;
};
} // namespace
