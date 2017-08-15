#ifndef HAL_H
#define HAL_H

#include <stdint.h>
#include <FastLED.h>

class CHAL
{
    public:
        typedef CPixelView<CRGB>*   CStrandView;

	protected:
		short		    m_StrandViewCount;
		CStrandView*	m_StrandViews;

		short           m_PhysicalStrandCount;
        CRGB*           m_LEDBuffer;

        uint32_t        m_Timer = 0;

	public:
		CHAL();
		virtual ~CHAL();

		void          CreateStrandViews();
		uint16_t      GetStrandViewCount();
    	CStrandView   GetStrandView(uint16_t inIndex);

		void	FlushLEDs();

		void ResetTime();
		uint32_t GetTime();
};

#endif // HAL_H
