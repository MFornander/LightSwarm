#include "effect.h"
#include "hal.h"
#include "debug.h"
#include <noise.h>

namespace LightSwarm {

CEffect::CEffect()
{
}


CEffect::effect_function CEffect::GetEffect(int inType)
{
	switch (inType)
	{
		default:
			return Effect_NULL;
			break;

		case 3: return Effect_Spark; break;
		case 4: return Effect_Rainbow; break;
		case 5: return Effect_Gadoosh; break;
		case 6: return Effect_Pulse; break;
		case 7: return Effect_Rain; break;
		case 8: return Effect_Spark2; break;
		case 9: return Effect_RGB; break;

	}
}


void CEffect::Effect_NULL(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView)
{
	//std::cout << inStrand->m_Index << ": NULL Effect" << std::endl;
	//INFO("Effect_NULLEffect\n");
}


void CEffect::Effect_Spark(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView)
{
	bool  theSparkForward = false;   // Get this later from Args
	int   theSparkSpeed = 100; // Get this later from Args
	int   thePeriod = inTime/theSparkSpeed;

	// Calculate the location of the spark and slow it near the end
	int   theSparkLoc = (inTime%theSparkSpeed) * inView->size() / theSparkSpeed;

	if (thePeriod % 3 == 0)
	{
		if (theSparkForward)
		{
			//(*inView)[theSparkLoc]*=CRGB::White;
			(*inView)[theSparkLoc].setColorCode(CRGB::White);
		}
		else
		{
			//(*inView)[inView->size()-1-theSparkLoc]*=CRGB::White;
			(*inView)[inView->size()-1-theSparkLoc].setColorCode(CRGB::White);
		}
	}
}


void CEffect::Effect_Spark2(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView)
{
	bool  theSparkForward = false;   // Get this later from Args
	int   theSparkSpeed = 69; // Get this later from Args
	int   thePeriod = inTime/theSparkSpeed;

	// Calculate the location of the spark and slow it near the end
	int   theSparkLoc = (inTime%theSparkSpeed) * inView->size() / theSparkSpeed;

	if (thePeriod % 1 == 0)
	{
		if (theSparkForward)
		{
			//(*inView)[theSparkLoc]*=CRGB::White;
			(*inView)[theSparkLoc].setColorCode(CRGB::DeepPink);
		}
		else
		{
			//(*inView)[inView->size()-1-theSparkLoc]*=CRGB::White;
			(*inView)[inView->size()-1-theSparkLoc].setColorCode(CRGB::DeepPink);
		}
	}
}


void CEffect::Effect_Rainbow(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView)
{
	int     theRainbowMovementDir = -1;   // Get this later from Args
	bool    thePulseFlag = true;   // Get this later from Args

	inView->fill_rainbow(inTime*theRainbowMovementDir, 256.0f/inView->size());

	if (thePulseFlag)
		inView->fadeLightBy( sin8_C(inTime)/2 + 100 );
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
	CHSV theColor(70, 1, inTime%255);

	//std::cout << inStrand->m_Index << ": Pulse" << std::endl;
	//INFO("Effect_Pulse\n");
	inView->fill_solid(theColor);
}


void CEffect::Effect_Rain(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView)
{
//OLD RAIN
/*
	totalTime += frame.timeDelta;
	//cout << "total time: " << totalTime << "\n";

	int dropCount = 100 * Input(Pot1);
	float hueShift = Input(Pot2) - 0.5f;

	bool dropAdded = false;
	for (int dropIndex = 0; dropIndex < dropCount; ++dropIndex) {
		RainDrop& drop = mDrops[dropIndex];
		int led = 100 * (totalTime - drop.birth)  / 3 ;
		//cout << "led: " << led << "\n";
		if (led < 100 + drop.tail) {
			for (uint32_t tailIndex = 0; tailIndex < drop.tail; ++tailIndex) {
				float brightness = (255 - tailIndex * 255 / drop.tail) / 255.0f;
				int32_t newLed = led - tailIndex;
				if (newLed >= 0 && newLed < static_cast<int32_t>(JellyPixel::kLedCount)) {
					pixels[drop.strip][newLed].h = drop.hue;
					pixels[drop.strip][newLed].s = drop.saturation;
					pixels[drop.strip][newLed].v = brightness;
				}
			}
		}

		if (led >= 100 + drop.tail && !dropAdded && (random() & 0xF)) {
			drop.birth = totalTime;
			drop.strip = random() % JellyPixel::kStrandCount;
			drop.hue = (112 + random() % 96) / 255.0f + hueShift;
			drop.saturation = (64 + random() % 160 ) / 255.0f;
			drop.tail = 5 + random() % 40;
			dropAdded = true;
		}
	}
*/
}


void CEffect::Effect_PerlinTest(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView)
{
	const int ledCount = inView->size();
	const uint8_t  kOctaves = 2;
	const uint32_t kPosition = inTime*10;
	const int      kScale = 2000;
	const uint32_t theTime = inTime*500;

	CRGB perlin[ledCount];
	uint8_t raw[ledCount];

	memset(raw, 0, ledCount);
	fill_raw_noise16into8(raw, ledCount, kOctaves, kPosition, kScale, theTime);

	for (int i = 0; i < ledCount; i++)
		perlin[i] = ColorFromPalette( RainbowColors_p, raw[i], 31); //CHSV(0,0,raw[i]);

	//inView->fill_solid(perlin[0]);

	//INFO("RGB:%u,%u,%u  count:%d oct:%u pos:%u scale:%u time:%u\n",
	//    perlin[0].r, perlin[0].g,  perlin[0].b,
	//    ledCount, kOctaves, kPosition, kScale, theTime);

	inView->operator=(CPixelView<CRGB>(perlin, ledCount));
}


void CEffect::Effect_RGB(uint32_t inTime, uint8_t* inArgs, CHAL::CStrandView inView)
{
	uint8_t theHue = inTime / 100;
	CHSV theColor(theHue, 255, 255);


	inView->fill_solid(theColor);
}
}
