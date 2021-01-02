#pragma once

#include "hal.h"
#include "presentation.h"
#include "strand.h"
#include <list>

namespace LightSwarm {

struct SBinding
{
    CPresentation*  m_Presentation;
    CStrand*        m_Strands;
};

class CVunsq final
{
public:
    CVunsq(CHAL& inHAL);

    void AddPresentation(CPresentation* inPresentation, int32_t inStrandOffset, uint8_t inBrightness = 255);
    void FreePresentations();
    void Step(uint32_t inTime);

protected:
    CHAL&               m_HAL;
    uint16_t            m_StrandCount;
    std::list<SBinding> m_Bindings;
};
}
