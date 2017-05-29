
#ifndef __SWARMDEBUG__
#define __SWARMDEBUG__


class SwarmDebug
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
