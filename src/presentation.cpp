#include "presentation.h"
#include "timeline.h"
#include "strand.h"
#include "debug.h"

namespace LightSwarm {

CPresentation::CPresentation()
{
    // Create an empty presentation that we can add strands to in the future
    m_Index = new SStrandEntry[MAX_PRESENTATION_STRANDS];
    for (uint8_t theStrand = 0; theStrand < m_PresentationStrandCount; theStrand++)
    {
        m_Index[theStrand].m_Offset = 0;
        m_Index[theStrand].m_ByteCount = 0;
        m_Index[theStrand].m_Sequence = nullptr;
    }
}

CPresentation::~CPresentation()
{
    for (uint8_t theStrand = 0; theStrand < m_PresentationStrandCount; theStrand++)
        delete [] m_Index[theStrand].m_Sequence;

    delete [] m_Index;
    m_Index = nullptr;
}

void CPresentation::CreateSequence(uint8_t inEffect, char*& outSequence, uint32_t& outByteCount)
{
    outByteCount = sizeof(uint32_t)+sizeof(SEvent);
    outSequence = new char[outByteCount];
    SEvent* theEvent = reinterpret_cast<SEvent*>(outSequence + sizeof(uint32_t));

    reinterpret_cast<uint32_t*>(outSequence)[0] = 1; //EventCount
    theEvent->m_EffectID = inEffect;
    theEvent->m_Start=0;
    theEvent->m_ArgCount = 0;
    theEvent->m_Speed = 1.0f;
}

void CPresentation::AddStrand(char* inSequence, uint32_t inByteCount)
{
    if (m_PresentationStrandCount>=MAX_PRESENTATION_STRANDS)
        return;

    m_Index[m_PresentationStrandCount].m_ByteCount = inByteCount;
    m_Index[m_PresentationStrandCount].m_Sequence = inSequence;

    m_PresentationStrandCount++;
}

CTimeline* CPresentation::CreateTimeline(short inStrandIndex)
{
    // Wrap the stand index to the number of strands in this presentation
    short   theWrappedIndex = inStrandIndex % m_PresentationStrandCount;

    return new CTimeline(m_Index[theWrappedIndex].m_Sequence, m_Index[theWrappedIndex].m_ByteCount);
}
}
