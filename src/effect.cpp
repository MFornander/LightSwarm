#include "effect.h"
#include "hal.h"
#include "debug.h"
#include <FastLED.h>

CEffect::CEffect()
{

}

CEffect::effect_function CEffect::GetEffect(EEffectType inType)
{
	switch (inType)
	{
		case EFallingStripe:
			return Effect_FallingStripe;
			break;
		case EGadoosh:
			return Effect_Gadoosh;
			break;
		case EPulse:
			return Effect_Pulse;
			break;
		default:
			return Effect_NULLEffect;
			break;
	}
}

void CEffect::Effect_FallingStripe(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView)
{
    //INFO("Effect_FallingStripe\n");
    //inView->fill_rainbow(inTime, 256.0f/inView->size());
    //return;

    //inView->fill_solid(CRGB::Black);
    int   theSparkLoc = (inTime%100) * inView->size() / 100;
    (*inView)[theSparkLoc].setColorCode(CRGB::White);

    //(*inView)[inTime%inView->size()].setColorCode(CRGB::White);

    //inView->operator[](inTime%inView->size()).setColorCode(CRGB::DeepPink);
    //inView->fill_gradient_RGB(CRGB::DeepPink, CRGB::Black, CRGB::Black);

    //(*inView)(10, 20).fill_solid(CRGB::DeepPink);
    //fill_gradient_RGB(CRGB::DeepPink, CRGB::Black);
        //fill_gradient_RGB(inStrand->m_PixelBuffer, 0, theStart, 20, theEnd);

	//std::cout << inStrand->m_Index << ": Falling Stripe" << std::endl;
    //INFO("%d: Effect_FallingStripe\n", inView->m_Index);
    //fill_solid( inStrand->m_PixelBuffer, inStrand->m_PixelCount, theStart);
    //fill_gradient_RGB(inStrand->m_PixelBuffer, 0, theStart, 20, theEnd);
    //fill_gradient_RGB(inStrand->m_PixelBuffer, inTime%inStrand->m_PixelCount, theStart, (inTime+20)%inStrand->m_PixelCount, theEnd);
}

void CEffect::Effect_Gadoosh(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView)
{
    //INFO("Effect_Gadoosh\n");
    inView->fill_rainbow(inTime, 256.0f/inView->size());
    inView->fadeLightBy(sin8_C(inTime)/2+100);
    /*
    uint8_t theDelta = 256.0f/inStrand->m_PixelCount;

    if ( inStrand->m_PhysicalDirection < 0 )
    {
        inTime += theDelta * inStrand->m_PixelCount;
        theDelta = 256 - theDelta;
    }

	//std::cout << inStrand->m_Index << ": Gadoosh" << std::endl;
    INFO("%d: Effect_Gadoosh\n", inStrand->m_Index);
	fill_rainbow(inStrand->m_PixelBuffer, inStrand->m_PixelCount, inTime, theDelta);
    */
}

void CEffect::Effect_Pulse(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView)
{
    CHSV theColor(0, 255, inTime%255);

	//std::cout << inStrand->m_Index << ": Pulse" << std::endl;
    //INFO("Effect_Pulse\n");
    inView->fill_solid(theColor);
}

void CEffect::Effect_NULLEffect(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView)
{
	//std::cout << inStrand->m_Index << ": NULL Effect" << std::endl;
    //INFO("Effect_NULLEffect\n");
}
