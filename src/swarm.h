#include <painlessMesh.h>

class swarm : public painlessMesh
{
    public:
        swarm();
        virtual ~swarm();

        virtual void update();

        // Callbacks for mesh control
        static void receivedCallback(uint32_t from, String &msg);
        static void newConnectionCallback(uint32_t nodeId);
        void changedConnectionCallback();
        void nodeTimeAdjustedCallback(int32_t offset);
        static void delayReceivedCallback(uint32_t from, int32_t delay);
};
