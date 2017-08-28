#ifndef VUNSQ_H
#define VUNSQ_H

#include "hal.h"
#include "presentation.h"
#include "strand.h"

struct SBinding
{
	CPresentation*	m_Presentation;
	CStrand*		m_Strands;
	};

class CVunsq
{
protected:
	CHAL			m_HAL;
	uint16_t		m_StrandCount;

	SimpleList<SBinding>    m_Bindings;

public:
	CVunsq();

	void AddPresentation(CPresentation* inPresentation, int32_t inStrandOffset, uint8_t inBrightness = 255);
	void Step(uint32_t inTime);
};

#endif // VUNSQ_H
