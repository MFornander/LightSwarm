
#ifndef __SWARMDEBUG__
#define __SWARMDEBUG__


class SwarmDebug
{
public:
    SwarmDebug();

    void SetLed(bool on = true);

    void Info(char* message);
    void Warn(char* message);
    void Error(char* message);
};

#endif
