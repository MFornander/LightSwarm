#include <Arduino.h>

#include "hal.h"
#include "debug.h"
#include "control.h"
#include "network.h"
#include "presentation.h"
#include "version.h"
#include "vunsq.h"

using namespace LightSwarm;

class MainHelper
{
public:
    MainHelper()
    : hal(network.GetNodeID())
    , player(hal)
    , control(network, player)
    {}

    void Update()
    {
        control.Update();
    }
private:
    Network         network;
    CHAL            hal;
    CVunsq          player;
    Control         control;
};

std::shared_ptr<MainHelper> mainHelperPtr;

void report()
{
    //Debug::GetSingleton().SetLed( network.GetNodeCount() == 0 );
    INFO("%s - id:<%x> fps:%d time:%x nodes:%u\n",// stab:%d frame:%u retry:%u\n",
         Version::BUILD, 0, 0, 0, 0, 0, 0//network.GetNodeID(), LEDS.getFPS(), network.GetTime(), network.GetNodeCount() + 1//,
         //network.GetStability(), _frame_cnt, _retry_cnt);
    );
}

void setup()
{
    mainHelperPtr = std::make_shared<MainHelper>();
}

void loop()
{
    EVERY_N_SECONDS(1) { report(); }
    mainHelperPtr->Update();
}
