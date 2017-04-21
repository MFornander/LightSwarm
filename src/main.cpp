//#include <RH_RF95.h>
//#define FASTLED_FORCE_SOFTWARE_PINS
#include <Arduino.h>
#define FASTLED_ESP8266_D1_PIN_ORDER
//#define FASTLED_INTERRUPT_RETRY_COUNT 0
//#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <painlessMesh.h>

#define LED_BUILTIN 2

#define NUM_LEDS 240
#define DATA_PIN 4
CRGB leds[NUM_LEDS];

#define   MESH_SSID       "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

painlessMesh  mesh;
bool calc_delay = false;
SimpleList<uint32_t> nodes;
uint32_t sendMessageTime = 0;


void receivedCallback(uint32_t from, String &msg)
{
    Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId)
{
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback()
{
    Serial.printf("Changed connections %s\n", mesh.subConnectionJson().c_str());

    nodes = mesh.getNodeList();

    Serial.printf("Num nodes: %d\n", nodes.size());
    Serial.printf("Connection list:");

    SimpleList<uint32_t>::iterator node = nodes.begin();
    while (node != nodes.end())
    {
        Serial.printf(" %u", *node);
        node++;
    }
    Serial.println();
    calc_delay = true;
}

void nodeTimeAdjustedCallback(int32_t offset)
{
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void delayReceivedCallback(uint32_t from, int32_t delay)
{
    Serial.printf("Delay to node %u is %d us\n", from, delay);
}

void setup()
{
    //WiFi.setSleepMode(WIFI_NONE_SLEEP);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // Ensure serial flash is not interfering with radio communication on SPI bus
    Serial.begin(115200);
    while (!Serial) ; // Wait for serial port to be available

    FastLED.addLeds<WS2811_PORTA, 1, GRB>(leds, NUM_LEDS);
    //FastLED.addLeds<WS2813, DATA_PIN, GRB>(leds, NUM_LEDS);
    set_max_power_in_volts_and_milliamps(5, 4000);

    mesh.setDebugMsgTypes(ERROR | DEBUG);  // set before init() so that you can see startup messages
    //mesh.setDebugMsgTypes(ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE );

    mesh.init(MESH_SSID, MESH_PASSWORD, MESH_PORT);
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
    mesh.onNodeDelayReceived(&delayReceivedCallback);

    randomSeed(analogRead(A0));

}

void debug()
{
    EVERY_N_SECONDS(5)
    {
        Serial.println(mesh.getNodeId());
        //Serial.println(LEDS.getFPS());
        static bool debug_blink = false;
        digitalWrite(LED_BUILTIN, debug_blink = !debug_blink ? HIGH : LOW);
    }
}

void animate( unsigned long anim_time )
{
    fill_rainbow(leds, NUM_LEDS, anim_time / 3, 5);
}


void swarm()
{
    bool error;

    mesh.update();

    // get next random time for send message
    if (sendMessageTime == 0)
    {
        sendMessageTime = mesh.getNodeTime() + random(1000000, 5000000);
    }

    // if the time is ripe, send everyone a message!
    if (sendMessageTime != 0 &&  (int) sendMessageTime - (int) mesh.getNodeTime() < 0)
    { // Cast to int in case of time rollover
        String msg = "Hello from node ";
        msg += mesh.getNodeId();
        error = mesh.sendBroadcast(msg + " myFreeMemory: " + String(ESP.getFreeHeap()));
        sendMessageTime = 0;

        if (calc_delay)
        {
            SimpleList<uint32_t>::iterator node = nodes.begin();
            while (node != nodes.end())
            {
                mesh.startDelayMeas(*node);
                node++;
            }
            calc_delay = false;
        }
    }
}

void loop()
{
    debug();
    swarm();

    EVERY_N_MILLISECONDS(10)
    {
        animate( millis() );
    }

    show_at_max_brightness_for_power();
    delay(1);
}
