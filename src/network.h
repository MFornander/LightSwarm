#ifndef __SWARM_NETWORK_H__
#define __SWARM_NETWORK_H__

#include <painlessMesh.h>

namespace LightSwarm {


class Network final
{
public:
    Network();
    ~Network();

    void Init(uint8_t channel = 0);
    void Update();
    uint32_t GetTime();
    uint32_t GetNodeID();
    uint32_t GetNodeCount();

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
    SimpleList<uint32_t>  m_nodes;
    uint32_t              m_sendMessageTime = 0;
};
} // namespace

#endif
