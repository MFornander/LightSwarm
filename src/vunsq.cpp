#include "vunsq.h"
#include "debug.h"
#include "presentation.h"

namespace LightSwarm {

CVunsq::CVunsq(CHAL& inHAL) :
    m_HAL(inHAL)
{
    // Set up the strands for this device
    m_HAL.CreateStrandViews();
    m_StrandCount = m_HAL.GetStrandViewCount();
}

void CVunsq::AddPresentation(CPresentation* inPresentation, int32_t inStrandOffset, uint8_t inBrightness)
{
    if (inPresentation == nullptr)
        return;

    CStrand* theStrands = new CStrand[m_StrandCount];

    // Connect each timeline of the presentation to the appropriate strand
    for (int16_t theStrandLoop = 0; theStrandLoop < m_StrandCount; theStrandLoop++)
    {
        theStrands[theStrandLoop].SetView(m_HAL.GetStrandView(theStrandLoop), inBrightness);
        theStrands[theStrandLoop].SetTimeline(inPresentation->CreateTimeline(theStrandLoop+inStrandOffset));
    }

    SBinding    theBinding{inPresentation, theStrands};
    m_Bindings.push_back(theBinding);

    // Reset the timer for this presentation
    // TODO(mf): Implement:
    //m_HAL.ResetTime();
}

void CVunsq::FreePresentations()
{
    for (auto theBinding: m_Bindings)
    {
        delete theBinding.m_Presentation;
        delete [] theBinding.m_Strands;
    }

    m_Bindings.clear();
}

void CVunsq::Step(uint32_t inTime)
{
    for (auto theBinding: m_Bindings)
    {
        // Step the animation of each strand associated with this Presentation
        for (int16_t theStrandLoop = 0; theStrandLoop < m_StrandCount; theStrandLoop++)
            theBinding.m_Strands[theStrandLoop].Step(inTime);
    }

    m_HAL.FlushLEDs();
}
}
