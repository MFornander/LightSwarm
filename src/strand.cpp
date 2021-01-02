#include "strand.h"
#include "hal.h"

namespace LightSwarm {


CStrand::CStrand()
{
    m_View = nullptr;
    m_Timeline = nullptr;
    m_Event = nullptr;
    m_Effect = CEffect::Effect_NULL;
}

CStrand::~CStrand()
{
    if (m_Timeline != nullptr)
        delete m_Timeline;
}

void CStrand::SetView(CHAL::CStrandView inView, uint8_t inBrightness)
{
    m_View = inView;
    m_Brightness = inBrightness;
}

void CStrand::SetTimeline(CTimeline* inTimeline)
{
    m_Timeline = inTimeline;
}

void CStrand::Step(uint32_t inTime)
{
    if (m_Timeline == nullptr || m_View == nullptr)
        return;

    // Establish the next effect if the event has been triggered
    SEvent* theNextEvent = m_Timeline->NextEvent(inTime);
    if (theNextEvent != nullptr)
    {
        m_Event = theNextEvent;
        m_Effect = CEffect::GetEffect(m_Event->m_EffectID);
    }

    // Advance the current effect
    if (m_Event)
        m_Effect(inTime, m_Event->m_Args, m_View);
}
}
