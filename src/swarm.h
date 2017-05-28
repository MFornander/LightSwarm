#ifndef __SWARM__
#define __SWARM__

#include <painlessMesh.h>


class Swarm : public painlessMesh
{
public:
    Swarm();
    virtual ~Swarm();

    virtual void update();

    // Callbacks for mesh control
    static void receivedCallback(uint32_t from, String& msg);
    static void newConnectionCallback(uint32_t nodeId);
    void changedConnectionCallback();
    void nodeTimeAdjustedCallback(int32_t offset);
    static void delayReceivedCallback(uint32_t from, int32_t delay);
};

#endif
