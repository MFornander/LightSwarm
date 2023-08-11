#include "hal.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <painlessMesh.h>
#include "debug.h"

namespace LightSwarm {

CHAL::CHAL(uint32_t inUUID) :
    m_UUID(inUUID)
{
    // TODO(mf): Disable sleep to minimize interrupts
    //esp_wifi_set_sleep_type(NONE_SLEEP_T);
    InitConfig();
}

CHAL::~CHAL()
{
}

void CHAL::InitConfig()
{
    INFO("UUID %x\n", m_UUID);

    const uint32_t  NODE_J0 = 0x7f3a2623; // J0
    const uint32_t  NODE_J1 = 0x34cf4164; // J1
    const uint32_t  NODE_J2 = 0x941eee8d; // J2
    const uint32_t  NODE_J3 = 0x7f3ad0bc; // J3
    const uint32_t  NODE_J4 = 0x7f3a37d1; // J4
    const uint32_t  NODE_J5 = 0x7f3a9bd4; // J5

    const uint32_t  NODE_D0 = 0x7f3a9c8b; // D0
    const uint32_t  NODE_D1 = 0x7f3a9b51; // D1
    const uint32_t  NODE_D2 = 0x7f0018a1; // D2
    const uint32_t  NODE_D3 = 0x34d04e4e; // D3
    const uint32_t  NODE_D4 = 0x7f3a9f79; // D4
    const uint32_t  NODE_D5 = 0x34cf362a; // D5

    const uint32_t  NODE_D6 = 0x7f3a9bb1; // D6

    switch (m_UUID)
    {
        case NODE_J0: m_Config.m_Position =  0; m_Config.m_Type = ENodeType::Type_Jelly; m_Config.m_Name = "J0"; break;
        case NODE_J1: m_Config.m_Position =  4; m_Config.m_Type = ENodeType::Type_Jelly; m_Config.m_Name = "J1"; break;
        case NODE_J2: m_Config.m_Position =  8; m_Config.m_Type = ENodeType::Type_Jelly; m_Config.m_Name = "J2"; break;
        case NODE_J3: m_Config.m_Position = 12; m_Config.m_Type = ENodeType::Type_Jelly; m_Config.m_Name = "J3"; break;
        case NODE_J4: m_Config.m_Position = 16; m_Config.m_Type = ENodeType::Type_Jelly; m_Config.m_Name = "J4"; break;
        case NODE_J5: m_Config.m_Position = 20; m_Config.m_Type = ENodeType::Type_Jelly; m_Config.m_Name = "J5"; break;

        case NODE_D0: m_Config.m_Position =  0; m_Config.m_Type = ENodeType::Type_Dong; m_Config.m_Name = "Beacon"; break;
        case NODE_D1: m_Config.m_Position =  4; m_Config.m_Type = ENodeType::Type_Dong; m_Config.m_Name = "D1"; break;
        case NODE_D2: m_Config.m_Position =  8; m_Config.m_Type = ENodeType::Type_Dong; m_Config.m_Name = "D2"; break;
        case NODE_D3: m_Config.m_Position = 12; m_Config.m_Type = ENodeType::Type_Dong; m_Config.m_Name = "D3"; break;
        case NODE_D4: m_Config.m_Position = 16; m_Config.m_Type = ENodeType::Type_Dong; m_Config.m_Name = "D4"; break;
        case NODE_D5: m_Config.m_Position = 20; m_Config.m_Type = ENodeType::Type_Dong; m_Config.m_Name = "D5"; break;

        case NODE_D6: m_Config.m_Position = 20; m_Config.m_Type = ENodeType::Type_Dong; m_Config.m_Name = "D6"; break;

        default:
            m_Config.m_Position =  0; m_Config.m_Type = ENodeType::Type_Testies; m_Config.m_Name = "Testies!";
            WARN("Unknown id= <%x> assuming Testies\n", m_UUID);
        break;
    }

    switch (m_Config.m_Type)
    {
        case Type_Jelly:
            m_Config.m_MilliampMax = 4000;
            m_Config.m_BackgroundLevel = 100;
            m_Config.m_PhysicalStrandCount = 4;
            m_Config.m_PhysicalStrandLEDCount = 244;
            m_StrandViewCount = 4;
            m_Config.m_StrandViewStartEnd.push_back({0, 243});
            m_Config.m_StrandViewStartEnd.push_back({244, 487});
            m_Config.m_StrandViewStartEnd.push_back({488, 731});
            m_Config.m_StrandViewStartEnd.push_back({732, 975});
        break;

        case Type_Dong:
            m_Config.m_MilliampMax = 1600;
            m_Config.m_BackgroundLevel = 70;
            m_Config.m_PhysicalStrandCount = 2;
            m_Config.m_PhysicalStrandLEDCount = 150;
            m_StrandViewCount = 4;
            m_Config.m_StrandViewStartEnd.push_back({74, 0});
            m_Config.m_StrandViewStartEnd.push_back({75, 149});
            m_Config.m_StrandViewStartEnd.push_back({224, 150});
            m_Config.m_StrandViewStartEnd.push_back({225, 299});
        break;

        case Type_Testies:
            m_Config.m_MilliampMax = 4000;
            m_Config.m_BackgroundLevel = 70;
            m_Config.m_PhysicalStrandCount = 1;
            m_Config.m_PhysicalStrandLEDCount = 300;
            m_StrandViewCount = 2;
            m_Config.m_StrandViewStartEnd.push_back({149, 0});
            m_Config.m_StrandViewStartEnd.push_back({150, 299});
        break;

        default:
        break;
    }
}

const SNodeConfig& CHAL::GetConfig()
{
    return m_Config;
}

void CHAL::CreateStrandViews()
{
    set_max_power_in_volts_and_milliamps(5, m_Config.m_MilliampMax);

    m_LEDBuffer = new CRGB[m_Config.m_PhysicalStrandCount*m_Config.m_PhysicalStrandLEDCount];
    switch (m_Config.m_PhysicalStrandCount)
    {
        case 1: FastLED.addLeds<APA102, G26, G32>(m_LEDBuffer, m_Config.m_PhysicalStrandLEDCount); break;

        // 2 strands is a dong with WS2812 aka 2811 type LEDs
       // case 2: FastLED.addLeds<APA102, G26, G32>(m_LEDBuffer, m_Config.m_PhysicalStrandLEDCount); break;

        // 4 strands is jelly with WS2813 type LEDs
        //case 4: FastLED.addLeds<APA102, 4, GRB>(m_LEDBuffer, m_Config.m_PhysicalStrandLEDCount); break;

        default: ERRX("ILLEGAL NUMBER OF PHYSICAL STRANDS"); break;
    }

    m_StrandViews = new CStrandView[m_StrandViewCount];
    int theIndex = 0;
    for (auto theEntry: m_Config.m_StrandViewStartEnd)
    {
        m_StrandViews[theIndex++] = new CPixelView<CRGB>(m_LEDBuffer, theEntry.m_Start, theEntry.m_End);
        INFO("Create Strand View %d: %d, %d\n", theIndex, theEntry.m_Start, theEntry.m_End);
    }
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
}
