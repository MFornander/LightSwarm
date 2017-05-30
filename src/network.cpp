#include <functional>

#include "network.h"

#define MESH_SSID       "whateverYouLike"
#define MESH_PASSWORD   "somethingSneaky"
#define MESH_PORT       5555

namespace LightSwarm {


Network::Network()
{
    using namespace std::placeholders;

    // CAN THIS BE DONE IN THE CONSTRUCTOR???
    m_mesh.setDebugMsgTypes(ERROR);
    //m_mesh.setDebugMsgTypes(ERROR | DEBUG | STARTUP | CONNECTION);  // set before init() so that you can see startup messages
    //mesh.setDebugMsgTypes(ERROR | DEBUG | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE );


    m_mesh.onReceive(             std::bind(&Network::ReceivedCallback, this, _1, _2));
    m_mesh.onNewConnection(       std::bind(&Network::NewConnectionCallback, this, _1));
    m_mesh.onChangedConnections(  std::bind(&Network::ChangedConnectionCallback, this));
    m_mesh.onNodeTimeAdjusted(    std::bind(&Network::NodeTimeAdjustedCallback, this, _1));
    m_mesh.onNodeDelayReceived(   std::bind(&Network::DelayReceivedCallback, this, _1, _2));

    randomSeed(analogRead(A0));
}

Network::~Network()
{
}

void Network::Init(uint8_t channel)
{
    m_mesh.init(MESH_SSID, MESH_PASSWORD, MESH_PORT, STA_AP, AUTH_WPA2_PSK, channel);
}

void Network::Update()
{
    m_mesh.update();

    // get next random time for send message
    if (sendMessageTime == 0)
        sendMessageTime = m_mesh.getNodeTime() + random(1000000, 5000000);

    // if the time is ripe, send everyone a message!
    if (sendMessageTime != 0 && (int)sendMessageTime - (int)m_mesh.getNodeTime() < 0)
    {
        // Cast to int in case of time rollover
        String msg = "Hello from ";
        msg += String(m_mesh.getNodeId(), HEX);
        bool error = m_mesh.sendBroadcast(msg);
        sendMessageTime = 0;

        if (calc_delay)
        {
            SimpleList<uint32_t>::iterator node = nodes.begin();
            while (node != nodes.end())
            {
                m_mesh.startDelayMeas(*node);
                node++;
            }
            calc_delay = false;
        }
    }
}

uint32_t Network::GetTime()
{
    return m_mesh.getNodeTime();
}

uint32_t Network::GetNodeCount()
{
    return m_mesh.getNodeList().size();
}

void Network::Broadcast(const String& message)
{
    m_mesh.sendBroadcast(const_cast<String&>(message));
}

void Network::SetReceived(ReceivedCallbackT callback)
{
    m_mesh.onReceive(callback);
}

void Network::ReceivedCallback(uint32_t from, String& msg)
{
    Serial.printf("%x: Received msg=%s\n", m_mesh.getNodeId(), msg.c_str());
}

void Network::NewConnectionCallback(uint32_t nodeId)
{
    Serial.printf("--> startHere: New Connection, nodeId = %x\n", nodeId);
}

void Network::ChangedConnectionCallback()
{
    Serial.printf("Changed connections %s\n", m_mesh.subConnectionJson().c_str());

    nodes = m_mesh.getNodeList();

    Serial.printf("Num nodes: %d\n", nodes.size());
    Serial.printf("Connection list:");

    SimpleList<uint32_t>::iterator node = nodes.begin();
    while (node != nodes.end())
    {
        Serial.printf(" %u", *node);
        node++;
    }
    Serial.println();
    calc_delay = true;
}

void Network::NodeTimeAdjustedCallback(int32_t offset)
{
    Serial.printf("Adjusted time %u. Offset = %d\n", m_mesh.getNodeTime(), offset);
}

void Network::DelayReceivedCallback(uint32_t from, int32_t delay)
{
    Serial.printf("Delay to node %u is %d us\n", from, delay);
}

} // namespace
