#ifndef STRAND_H
#define STRAND_H

#include "timeline.h"
#include "effect.h"

class CStrand
{
    protected:
		CHAL::CStrandView		    m_View;
		CTimeline*					m_Timeline;
		SEvent*						m_Event;
		CEffect::effect_function	m_Effect;

    public:
		CStrand();
		virtual ~CStrand();

		void SetView(CHAL::CStrandView inView);
		void SetTimeline(CTimeline* inTimeline);
		void Step(uint32_t inTime);
};

#endif // STRAND_H
