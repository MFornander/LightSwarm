#pragma once

#define FASTLED_INTERNAL
#define FASTLED_ESP8266_D1_PIN_ORDER
#define FASTLED_DEBUG_COUNT_FRAME_RETRIES
#define FASTLED_INTERRUPT_RETRY_COUNT 0
//#define FASTLED_ALLOW_INTERRUPTS 0
#include <Arduino.h>
#include <FastLED.h>
#include <stdint.h>
#include <list>

namespace LightSwarm {

enum ENodeType
{
    Type_Controller = 0,
    Type_Jelly,
    Type_Dong,
    Type_Testies,
    Type_Other
};

struct SStartEnd
{
    int     m_Start;
    int     m_End;
};

struct SNodeConfig
{
    int                     m_Position = 0;
    int                     m_Type = Type_Other;
    String                  m_Name = "None";
    int                     m_MilliampMax = 4000;
    int                     m_BackgroundLevel = 100;
    int                     m_PhysicalStrandCount = 0;
    int                     m_PhysicalStrandLEDCount = 100;
    std::list<SStartEnd>    m_StrandViewStartEnd;
};

class CHAL
{
public:
    typedef CPixelView<CRGB>*   CStrandView;

protected:
    const short     MAX_PHYSICALSTRAND_COUNT = 4;

    uint32_t        m_UUID = 0;
    SNodeConfig     m_Config;
    short           m_StrandViewCount;
    CStrandView*    m_StrandViews;
    short           m_PhysicalStrandCount;

    CRGB*           m_LEDBuffer;
    CRGB            m_LEDButton{255,0,0};

public:
    CHAL(uint32_t inUUID);
    virtual ~CHAL();

    void                InitConfig();
    const SNodeConfig&  GetConfig();

    void          CreateStrandViews();
    uint16_t      GetStrandViewCount();
    CStrandView   GetStrandView(uint16_t inIndex);

    void          FlushLEDs();
    void          SetLEDButton(CRGB const& color); 
};
}
