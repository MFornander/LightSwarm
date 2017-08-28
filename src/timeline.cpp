#include "timeline.h"

namespace LightSwarm {


CTimeline::CTimeline(char* inSequence, uint32_t inByteCount)
{
	m_Sequence = inSequence;
	m_SequenceByteCount = inByteCount;

	// Jump past the event count (4 bytes)
	m_EventCount = *((uint32_t*)m_Sequence);
	m_NextEvent = reinterpret_cast<SEvent*>(m_Sequence + sizeof(long));
}

CTimeline::~CTimeline()
{
	//if (m_Sequence)
	//	delete[] m_Sequence;
}

SEvent* CTimeline::NextEvent(uint32_t inTime)
{
	SEvent*		theReturnEvent = nullptr;

	// Check if the incoming time triggers the next event
	if (m_NextEvent != nullptr && inTime >= m_NextEvent->m_Start)
	{
		theReturnEvent = m_NextEvent;

		// Advance the to the next event in the sequence.  Set to null at the end of sequence
		m_NextEvent = reinterpret_cast<SEvent*>(((char*)m_NextEvent) + sizeof(SEvent) + m_NextEvent->m_ArgCount);
		if ((char*)m_NextEvent >= m_Sequence + m_SequenceByteCount)
			m_NextEvent = nullptr;
	}

	return theReturnEvent;
}
}
