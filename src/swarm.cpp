#include "swarm.h"
#include <functional>

#define   MESH_SSID       "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

bool calc_delay = false;
SimpleList<uint32_t> nodes;
uint32_t sendMessageTime = 0;

swarm::swarm()
{
    using namespace std::placeholders;

    // CAN THIS BE DONE IN THE CONSTRUCTOR???
    setDebugMsgTypes(ERROR | DEBUG);  // set before init() so that you can see startup messages
    //mesh.setDebugMsgTypes(ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE );

    init(MESH_SSID, MESH_PASSWORD, MESH_PORT);
    onReceive(&receivedCallback);
    onNewConnection(&newConnectionCallback);
    //onChangedConnections(&changedConnectionCallback);
    onChangedConnections(std::bind(&swarm::changedConnectionCallback, this));
    onNodeTimeAdjusted(std::bind(&swarm::nodeTimeAdjustedCallback, this, _1));
    onNodeDelayReceived(&delayReceivedCallback);

    randomSeed(analogRead(A0));
}

swarm::~swarm()
{
}

void swarm::update()
{
    bool error;

    painlessMesh::update();

    // get next random time for send message
    if (sendMessageTime == 0)
        sendMessageTime = getNodeTime() + random(1000000, 5000000);

    // if the time is ripe, send everyone a message!
    if (sendMessageTime != 0 &&  (int) sendMessageTime - (int) getNodeTime() < 0)
    { // Cast to int in case of time rollover
        String msg = "Hello from " NAME;
        error = sendBroadcast(msg);
        sendMessageTime = 0;

        if (calc_delay)
        {
            SimpleList<uint32_t>::iterator node = nodes.begin();
            while (node != nodes.end())
            {
                startDelayMeas(*node);
                node++;
            }
            calc_delay = false;
        }
    }
}

void swarm::receivedCallback(uint32_t from, String &msg)
{
    Serial.printf(NAME ": Received msg=%s\n", msg.c_str());
}

void swarm::newConnectionCallback(uint32_t nodeId)
{
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void swarm::changedConnectionCallback()
{
    Serial.printf("Changed connections %s\n", subConnectionJson().c_str());

    nodes = getNodeList();

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

void swarm::nodeTimeAdjustedCallback(int32_t offset)
{
    Serial.printf("Adjusted time %u. Offset = %d\n", getNodeTime(), offset);
}

void swarm::delayReceivedCallback(uint32_t from, int32_t delay)
{
    Serial.printf("Delay to node %u is %d us\n", from, delay);
}
