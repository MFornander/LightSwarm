
#ifndef __SWARM_DEBUG_H__
#define __SWARM_DEBUG_H__


class SwarmDebug final
{
public:
    SwarmDebug();

    void SetLed(bool on = true);

    void Trace(const char* format, ...);
    void Info(const char* format, ...);
    void Warn(const char* format, ...);
    void Error(const char* format, ...);
};

#endif
