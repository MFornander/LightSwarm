#ifndef VUNSQ_H
#define VUNSQ_H

class CPresentation;

#include "hal.h"
#include "strand.h"

class CVunsq
{
	protected:
		CHAL			m_HAL;
		uint16_t		m_StrandCount;
		CStrand*		m_Strands;
		CPresentation*	m_Presentation;

    public:
        CVunsq();

		void SetPresentation(CPresentation* inPresentation);
		void Step();
		
};

#endif // VUNSQ_H
