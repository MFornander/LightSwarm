#pragma once
#include <stdint.h>

namespace LightSwarm {

//TODO under GCC try struct __attribute__ ((packed))
#pragma pack(push, 1)
struct SEvent
{
    uint8_t     m_EffectID;
    uint32_t    m_Start;
    float       m_Speed;
    uint8_t     m_ArgCount;
    uint8_t     m_Args[];       // NOTE: Will not be included in "sizeof" calculations
};
#pragma pack(pop)

class CTimeline final
{
public:
    CTimeline(char* inSequence, uint32_t inByteCount);
    ~CTimeline();

    SEvent* NextEvent(uint32_t inTime);

protected:
    char*       m_Sequence = nullptr;
    uint32_t    m_SequenceByteCount;
    uint32_t    m_EventCount;
    SEvent*     m_NextEvent;
};
}
