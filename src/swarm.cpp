#include "swarm.h"
#include <functional>

#define   MESH_SSID       "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555


SwarmNetwork::SwarmNetwork()
{
    using namespace std::placeholders;

    // CAN THIS BE DONE IN THE CONSTRUCTOR???
    m_mesh.setDebugMsgTypes(ERROR);
    //m_mesh.setDebugMsgTypes(ERROR | DEBUG | STARTUP | CONNECTION);  // set before init() so that you can see startup messages
    //mesh.setDebugMsgTypes(ERROR | DEBUG | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE );


    m_mesh.onReceive(             std::bind(&SwarmNetwork::ReceivedCallback, this, _1, _2));
    m_mesh.onNewConnection(       std::bind(&SwarmNetwork::NewConnectionCallback, this, _1));
    m_mesh.onChangedConnections(  std::bind(&SwarmNetwork::ChangedConnectionCallback, this));
    m_mesh.onNodeTimeAdjusted(    std::bind(&SwarmNetwork::NodeTimeAdjustedCallback, this, _1));
    m_mesh.onNodeDelayReceived(   std::bind(&SwarmNetwork::DelayReceivedCallback, this, _1, _2));

    randomSeed(analogRead(A0));
}

SwarmNetwork::~SwarmNetwork()
{
}

void SwarmNetwork::Init(uint8_t channel)
{
    m_mesh.init(MESH_SSID, MESH_PASSWORD, MESH_PORT, STA_AP, AUTH_WPA2_PSK, channel);
}

void SwarmNetwork::Update()
{
    m_mesh.update();

    // get next random time for send message
    if (sendMessageTime == 0)
        sendMessageTime = m_mesh.getNodeTime() + random(1000000, 5000000);

    // if the time is ripe, send everyone a message!
    if (sendMessageTime != 0 && (int)sendMessageTime - (int)m_mesh.getNodeTime() < 0)
    {
        // Cast to int in case of time rollover
        String msg = "Hello from " NAME " ";
        msg += m_mesh.getNodeId();
//        bool error = m_mesh.sendBroadcast(msg);
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

uint32_t SwarmNetwork::GetTime()
{
    return m_mesh.getNodeTime();
}

uint32_t SwarmNetwork::GetNodeCount()
{
    return m_mesh.getNodeList().size();
}

void SwarmNetwork::Broadcast(const String& message)
{
    m_mesh.sendBroadcast(const_cast<String&>(message));
}

void SwarmNetwork::SetReceived(ReceivedCallbackT callback)
{
    m_mesh.onReceive(callback);
}

void SwarmNetwork::ReceivedCallback(uint32_t from, String& msg)
{
    Serial.printf(NAME ": Received msg=%s\n", msg.c_str());
}

void SwarmNetwork::NewConnectionCallback(uint32_t nodeId)
{
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void SwarmNetwork::ChangedConnectionCallback()
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

void SwarmNetwork::NodeTimeAdjustedCallback(int32_t offset)
{
    Serial.printf("Adjusted time %u. Offset = %d\n", m_mesh.getNodeTime(), offset);
}

void SwarmNetwork::DelayReceivedCallback(uint32_t from, int32_t delay)
{
    Serial.printf("Delay to node %u is %d us\n", from, delay);
}
