#include <ArduinoJson.h>
#include "control.h"
#include "debug.h"
#include "network.h"
#include "vunsq.h"

namespace LightSwarm {

Control::Control(Network& inNetwork, CVunsq& inPlayer) :
    m_Network(inNetwork),
    m_Player(inPlayer)
{
    using namespace std::placeholders;

    m_Network.SetReceived( std::bind(&Control::OnMessage, this, _1, _2) );

    const int kStartAnimation = 0;
    m_CurrrentAnimation = kStartAnimation + 1;
    SelectAnimation(kStartAnimation);
}

void Control::Update()
{
    // Handle single and long click of the built in button on atom-lite
    switch (m_MainButton.Poll())
    {
        case EButtonClick::Click_Single:
            INFO(" [CTRL] SINGLE CLICK\n");
        break;
        case EButtonClick::Click_Long:
            INFO(" [CTRL] LONG CLICK\n");
        break;
        case EButtonClick::Click_None:
        default:
        break;
    }

    m_Network.Update();

    uint32_t theTimeDivider;
    switch (m_Speed)
    {
        default:
        case 0: theTimeDivider = 10000; break;
        case 1: theTimeDivider = 50000; break;
        case 2: theTimeDivider = 3000; break;
    }

    m_Player.Step(m_Network.GetTime() / theTimeDivider);
}

void Control::OnMessage(uint32_t inFromNodeID, const String& inMessage)
{
    INFO(" [CTRL] <%x>:  Received msg=%s from=%x\n", m_Network.GetNodeID(), inMessage.c_str(), inFromNodeID);

    StaticJsonDocument<kMaxJson> jsonDoc;
    auto error = deserializeJson(jsonDoc, inMessage.c_str());
    if (error)
    {
        WARN("deserializeJson() failed with code: %s", error.c_str());
        return;
    }

    if (!jsonDoc.containsKey("msg"))
    {
        WARN("Not a message\n");
        return;
    }

    const char* msg = jsonDoc["msg"];
    if (0 == strcmp(msg, "SetAnim"))
    {
        SelectAnimation(jsonDoc["idx"]);
        SelectSpeed(jsonDoc["spd"]);
    }
}

void Control::SelectAnimation(int inIndex)
{
    const int kPresentationCount = 2;
    inIndex %= kPresentationCount;

    if (inIndex == m_CurrrentAnimation)
        return;
    m_CurrrentAnimation = inIndex;

    m_Player.FreePresentations();

    char*           theSequence;
    uint32_t        theByteCount;
    CPresentation*  thePresentation;

    INFO(" [CTRL] Switching to pres=%u\n", inIndex);

    switch (inIndex)
    {
        case 1:
        {
            thePresentation  = new CPresentation();
            CPresentation::CreateSequence(9, theSequence, theByteCount);
            thePresentation->AddStrand(theSequence, theByteCount);
            m_Player.AddPresentation(thePresentation, m_Network.GetNodeOffset());

            break;
        }

        default:
        {
            CPresentation::CreateSequence(4, theSequence, theByteCount);
            thePresentation  = new CPresentation();
            thePresentation->AddStrand(theSequence, theByteCount);
            m_Player.AddPresentation(thePresentation, m_Network.GetNodeOffset());

            CPresentation::CreateSequence(3, theSequence, theByteCount);
            thePresentation = new CPresentation();
            thePresentation->AddStrand(theSequence, theByteCount);
            m_Player.AddPresentation(thePresentation, m_Network.GetNodeOffset());

            CPresentation::CreateSequence(8, theSequence, theByteCount);
            thePresentation  = new CPresentation();
            thePresentation->AddStrand(theSequence, theByteCount);
            m_Player.AddPresentation(thePresentation, m_Network.GetNodeOffset());

            break;
        }
    }
}

void Control::SelectSpeed(int inSpeed)
{
    m_Speed = inSpeed % 3;
    INFO(" [CTRL] Switching to speed=%d\n", m_Speed);
}
}
