#ifndef STRAND_H
#define STRAND_H

#include "timeline.h"
#include "effect.h"

namespace LightSwarm {

class CStrand final
{

public:
	CStrand();
	~CStrand();

	void SetView(CHAL::CStrandView inView, uint8_t inBrightness);
	void SetTimeline(CTimeline* inTimeline);
	void Step(uint32_t inTime);

protected:
	CHAL::CStrandView		    m_View;
	CTimeline*					m_Timeline;
	SEvent*						m_Event;
	CEffect::effect_function	m_Effect;
	uint8_t                     m_Brightness = 0; // until we create content hack this to be an effect offset
};
}

#endif // STRAND_H
