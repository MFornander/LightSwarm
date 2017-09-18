#pragma once
#include <stdint.h>
#include "hal.h"

namespace LightSwarm {

class CEffect
{
public:
	typedef void(*effect_function)(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView);

	CEffect();

	static effect_function GetEffect(int inType);

	static void Effect_Spark(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView);
	static void Effect_Rainbow(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView);
	static void Effect_Gadoosh(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView);
	static void Effect_Pulse(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView);
	static void Effect_Rain(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView);
	static void Effect_PerlinTest(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView);
	static void Effect_Spark2(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView);
	static void Effect_NULL(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView);

	static void Effect_RGB(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView);
};
}
