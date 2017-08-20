#define FASTLED_ESP8266_D1_PIN_ORDER
#define FASTLED_DEBUG_COUNT_FRAME_RETRIES
#define FASTLED_INTERRUPT_RETRY_COUNT 0
//#define FASTLED_ALLOW_INTERRUPTS 0

#include "hal.h"

#include <Arduino.h>
#include <FastLED.h>
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <ArduinoJson.h>

extern "C" {
#include "user_interface.h"
#include "espconn.h"
}

#include "debug.h"

#define WEMOS_BUTTON D3
#define WEMOS_RGB    D2

CHAL::CHAL()
{
    SPIFFS.begin();
    InitConfig();

	// TODO(mf): Remove and refactor this to a dedicated animation class
	WiFi.setSleepMode(WIFI_NONE_SLEEP);
	set_max_power_in_volts_and_milliamps(5, 1600);

	// TODO(mf) Remove debug OTA hack
	pinMode(WEMOS_BUTTON, INPUT);
	pinMode(WEMOS_RGB, INPUT);
}

CHAL::~CHAL()
{
}

void CHAL::InitConfig()
{
    uint8_t theMAC[] = { 0,0,0,0,0,0 };
    wifi_get_macaddr(SOFTAP_IF, theMAC);
    uint32 theUUID = 0;

    theUUID |= theMAC[2] << 24; //Big endian (aka "network order"):
    theUUID |= theMAC[3] << 16;
    theUUID |= theMAC[4] << 8;
    theUUID |= theMAC[5];

    INFO("UUID %x\n",theUUID);

    const uint32_t  NODE_Jelly_0 = 0x00000000;
    const uint32_t  NODE_Jelly_1 = 0x000001;
    const uint32_t  NODE_Jelly_2 = 0x000002;
    const uint32_t  NODE_Jelly_3 = 0x000003;
    const uint32_t  NODE_Jelly_4 = 0x000004;
    const uint32_t  NODE_Jelly_5 = 0x000005;
    const uint32_t  NODE_Dong_0 = 0x7f3a9f79; //G
    const uint32_t  NODE_Dong_1 = 0x7f3a2623; //A
    const uint32_t  NODE_Dong_2 = 0x7f3a9b51; //F
    const uint32_t  NODE_Dong_3 = 0x000009;
    const uint32_t  NODE_Dong_4 = 0x000010;

    switch(theUUID)
    {
        case NODE_Jelly_0: m_Config.m_Position = 0; m_Config.m_Type = ENodeType::Type_Jelly; m_Config.m_Name = "J0"; break;
        case NODE_Jelly_1: m_Config.m_Position = 4; m_Config.m_Type = ENodeType::Type_Jelly; m_Config.m_Name = "J1"; break;
        case NODE_Jelly_2: m_Config.m_Position = 8; m_Config.m_Type = ENodeType::Type_Jelly; m_Config.m_Name = "J2"; break;
        case NODE_Jelly_3: m_Config.m_Position = 12; m_Config.m_Type = ENodeType::Type_Jelly; m_Config.m_Name = "J3"; break;
        case NODE_Jelly_4: m_Config.m_Position = 16; m_Config.m_Type = ENodeType::Type_Jelly; m_Config.m_Name = "J4"; break;
        case NODE_Jelly_5: m_Config.m_Position = 20; m_Config.m_Type = ENodeType::Type_Jelly; m_Config.m_Name = "J5"; break;

        case NODE_Dong_0: m_Config.m_Position = 0; m_Config.m_Type = ENodeType::Type_Dong; m_Config.m_Name = "D0"; break;
        case NODE_Dong_1: m_Config.m_Position = 4; m_Config.m_Type = ENodeType::Type_Dong; m_Config.m_Name = "D1"; break;
        case NODE_Dong_2: m_Config.m_Position = 8; m_Config.m_Type = ENodeType::Type_Dong; m_Config.m_Name = "D2"; break;
        case NODE_Dong_3: m_Config.m_Position = 12; m_Config.m_Type = ENodeType::Type_Dong; m_Config.m_Name = "D3"; break;
        case NODE_Dong_4: m_Config.m_Position = 16; m_Config.m_Type = ENodeType::Type_Dong; m_Config.m_Name = "D4"; break;

        default: m_Config.m_Position = 0; m_Config.m_Type = ENodeType::Type_Other; m_Config.m_Name = "Unknown"; break;
    }

    switch(m_Config.m_Type)
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
            m_Config.m_StrandViewStartEnd.push_back({0, 74});
            m_Config.m_StrandViewStartEnd.push_back({149, 75});
            m_Config.m_StrandViewStartEnd.push_back({150, 224});
            m_Config.m_StrandViewStartEnd.push_back({299, 225});
        break;
        default:
        break;
    }
}

/*void CHAL::LoadConfig()
{
    InitConfig();

    File configFile = SPIFFS.open("/node_config.json", "r");
    if (!configFile)
    {
        INFO("[OTA] No config file, using defaults.\n");
        return;
    }

    // Parse the Json from the file
    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    configFile.close();
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(buf.get());
    if (!root.success())
    {
        ERR("Failed to parse node config file\n");
        return;
    }

    m_Config.m_Name = root["name"].as<String>();
    m_Config.m_MilliampMax = root["milliampMax"];
    m_Config.m_BackgroundLevel = root["backgroundLevel"];
    m_Config.m_PhysicalStrandCount = root["physicalStrandCount"];
    m_Config.m_PhysicalStrandLEDCount = root["physicalStrandLEDCount"];

    INFO("CONFIG: %d, %d, %s, %d, %d, %d, %d\n",
            m_Config.m_Position, m_Config.m_Type, m_Config.m_Name.c_str(),
            m_Config.m_MilliampMax, m_Config.m_BackgroundLevel,
            m_Config.m_PhysicalStrandCount, m_Config.m_PhysicalStrandLEDCount);

    // Create the the start and end indices for the logical strands
    JsonArray&  theStartEnds = root["strandViewStartEnd"];
    m_StrandViewCount = theStartEnds.size();
    for (int theLoop=0; theLoop < m_StrandViewCount; theLoop++)
    {
        SStartEnd theStartEnd = {theStartEnds[theLoop]["start"], theStartEnds[theLoop]["end"]};
        m_Config.m_StrandViewStartEnd.push_back(theStartEnd);
    }
}
*/

const SNodeConfig& CHAL::GetConfig()
{
    return m_Config;
}

void CHAL::CreateStrandViews()
{
    m_LEDBuffer = new CRGB[m_Config.m_PhysicalStrandCount*m_Config.m_PhysicalStrandLEDCount];
    switch (m_Config.m_PhysicalStrandCount)
    {
        case 2: FastLED.addLeds<WS2811_PORTA, 2, GRB>(m_LEDBuffer, m_Config.m_PhysicalStrandLEDCount); break;
        case 4: FastLED.addLeds<WS2811_PORTA, 4, GRB>(m_LEDBuffer, m_Config.m_PhysicalStrandLEDCount); break;
        default: ERR("ILLEGAL NUMBER OF PHYSICAL STRANDS"); break;
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
