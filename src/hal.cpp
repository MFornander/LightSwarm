#include "hal.h"

#define FASTLED_ESP8266_D1_PIN_ORDER
#define FASTLED_DEBUG_COUNT_FRAME_RETRIES
//#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ALLOW_INTERRUPTS 0

#include <Arduino.h>
#include <FastLED.h>
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <FS.h>

#define WEMOS_BUTTON D3
#define WEMOS_RGB    D2

CHAL::CHAL()
{
	// TODO(mf): Remove and refactor this to a dedicated animation class
	WiFi.setSleepMode(WIFI_NONE_SLEEP);
	set_max_power_in_volts_and_milliamps(5, 4000);
	//FastLED.addLeds<WS2813_PORTA, NUM_STRANDS, GRB>(leds, NUM_LEDS);
	//memset8(leds, 0, NUM_LEDS*NUM_STRANDS * 3);

	// TODO(mf) Remove debug OTA hack
	pinMode(WEMOS_BUTTON, INPUT);
	pinMode(WEMOS_RGB, INPUT);

    SPIFFS.begin();
}

CHAL::~CHAL()
{
}

void CHAL::CreateStrandViews()
{
	// TODO: Load from configuration file

	// Dong sample data
	const short thePhysicalStrandCount = 2;
	const short thePhysicalLEDsPerStand = 150;
    m_LEDBuffer = new CRGB[thePhysicalStrandCount*thePhysicalLEDsPerStand];
    FastLED.addLeds<WS2811_PORTA, thePhysicalStrandCount, GRB>(m_LEDBuffer, thePhysicalLEDsPerStand);

	m_StrandViewCount = 4;
	m_StrandViews = new CStrandView[m_StrandViewCount];

    m_StrandViews[0] = new CPixelView<CRGB>(m_LEDBuffer, 0, 74);
    m_StrandViews[1] = new CPixelView<CRGB>(m_LEDBuffer, 149, 75);
    m_StrandViews[2] = new CPixelView<CRGB>(m_LEDBuffer, 150, 224);
    m_StrandViews[3] = new CPixelView<CRGB>(m_LEDBuffer, 299, 225);
}

uint16_t CHAL::GetStrandViewCount()
{
	return m_StrandViewCount;
}

CHAL::CStrandView CHAL::GetStrandView(uint16_t inIndex)
{
	if (inIndex < m_StrandViewCount && m_StrandViews)
		return m_StrandViews[inIndex];
	else
		return nullptr;
}

void CHAL::FlushLEDs()
{
	// Push buffers to HW
	show_at_max_brightness_for_power();
}

uint32_t CHAL::GetTime()
{
	// TODO: pass on network time
    return m_Timer++;
}

void CHAL::ResetTime()
{
    // TODO: implement
}
