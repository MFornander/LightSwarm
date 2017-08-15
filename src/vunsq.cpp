#include "vunsq.h"
#include "debug.h"
#include "presentation.h"

CVunsq::CVunsq()
{
	m_Presentation = nullptr;

	// Set up the strands for this device
	m_HAL.CreateStrandViews();
	m_StrandCount = m_HAL.GetStrandViewCount();
	m_Strands = new CStrand[m_StrandCount];
	for (int16_t theStrandLoop = 0; theStrandLoop < m_StrandCount; theStrandLoop++)
		m_Strands[theStrandLoop].SetView(m_HAL.GetStrandView(theStrandLoop));
}

void CVunsq::SetPresentation(CPresentation * inPresentation)
{
	if ( inPresentation == nullptr )
		return;

	m_Presentation = inPresentation;

	// Connect each timeline of the presentation to the appropriate strand
	for (int16_t theStrandLoop = 0; theStrandLoop < m_StrandCount; theStrandLoop++)
		m_Strands[theStrandLoop].SetTimeline(inPresentation->CreateTimeline(theStrandLoop));

	// Reset the timer for this presentation
	m_HAL.ResetTime();
}

void CVunsq::Step()
{
	uint32_t	theTime = m_HAL.GetTime();

	//std::cout << "TIME: " << theTime << std::endl;
    INFO("TIME: %d\n", theTime);

	// Connect each timeline of the presentation to the appropriate strand
	for (int16_t theStrandLoop = 0; theStrandLoop < m_StrandCount; theStrandLoop++)
    {
        INFO("Strand %d: ", theStrandLoop);
		m_Strands[theStrandLoop].Step(theTime);
    }

	m_HAL.FlushLEDs();
}
