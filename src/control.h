#ifndef __SWARM_CONTROL_H__
#define __SWARM_CONTROL_H__

namespace LightSwarm {

class Control final
{
    public:
        Control();

        void ProcessMessage(const String& message);
};
} // namespace

#endif
