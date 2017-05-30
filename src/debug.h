#ifndef __SWARM_DEBUG_H__
#define __SWARM_DEBUG_H__

namespace LightSwarm {

class Debug final
{
public:
    Debug();

    void SetLed(bool on = true);

    void Trace(const char* format, ...);
    void Info(const char* format, ...);
    void Warn(const char* format, ...);
    void Error(const char* format, ...);
};
} // namespace

#endif
