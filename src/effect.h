#ifndef EFFECT_H
#define EFFECT_H

#include <stdint.h>
#include "hal.h"

class CEffect
{
	public:
		typedef void(*effect_function)(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView);

		enum EEffectType
		{
			EFallingStripe=3,
			EGadoosh = 4,
			EPulse,
			EC,
			ENULL = 255
		};

		CEffect();

		static effect_function GetEffect(EEffectType inType);

		static void Effect_FallingStripe(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView);
		static void Effect_Gadoosh(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView);
		static void Effect_Pulse(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView);
		static void Effect_NULLEffect(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView);
};

#endif // EFFECT_H
