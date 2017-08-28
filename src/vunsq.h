#ifndef VUNSQ_H
#define VUNSQ_H

#include "hal.h"
#include "presentation.h"
#include "strand.h"

namespace LightSwarm {

struct SBinding
{
	CPresentation*	m_Presentation;
	CStrand*		m_Strands;
};

class CVunsq final
{

public:
	CVunsq();

	void AddPresentation(CPresentation* inPresentation, int32_t inStrandOffset, uint8_t inBrightness = 255);
	void FreePresentations();

	void Step(uint32_t inTime);

protected:
	CHAL			m_HAL;
	uint16_t		m_StrandCount;

	SimpleList<SBinding>    m_Bindings;

};
}

#endif // VUNSQ_H
