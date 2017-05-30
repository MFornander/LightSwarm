
#include <ArduinoJson.h>
#include <ESP8266httpUpdate.h>
#include <FS.h>

#include "ota.h"


#define CONFIG_NAME "/ota.json"
#define CONFIG_SIZE 200

SwarmOTA::SwarmOTA()
{
    SPIFFS.begin();
}


void SwarmOTA::RebootUpdate(const String& ssid, const String& pass, const String& host, const String& path)
{
    StaticJsonBuffer<CONFIG_SIZE> jsonBuffer;
    JsonObject& config = jsonBuffer.createObject();

    config["ssid"] = ssid;
    config["pass"] = pass;
    config["host"] = host;
    config["path"] = path;

    File configFile = SPIFFS.open(CONFIG_NAME, "w");
    if (configFile)
    {
        config.printTo(configFile);
        configFile.close();
        SPIFFS.end();

        Serial.println("[OTA] Rebooting in OTA mode...");
        ESP.restart();
    }


    Serial.println("[OTA] Failed to open config file for writing");
}



void SwarmOTA::TryUpdate()
{
    File configFile = SPIFFS.open(CONFIG_NAME, "r");
    if (!configFile)
    {
        Serial.println("[OTA] No config file. Leaving OTA.");
        return;
    }

    size_t size = configFile.size();
    if (size > CONFIG_SIZE)
    {
       Serial.printf("[OTA] Config file size (%d) is too large (%d)\n", (int)size, CONFIG_SIZE);
       return;
    }

    // Allocate a buffer to store contents of the file.
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    configFile.close();
    SPIFFS.remove(CONFIG_NAME);

    StaticJsonBuffer<CONFIG_SIZE> jsonBuffer;
    JsonObject& config = jsonBuffer.parseObject(buf.get());

    if (!config.success())
    {
        Serial.println("[OTA] Failed to parse config file");
        return;
    }

    const char* ssid = config["ssid"];
    const char* pass = config["pass"];
    const char* host = config["host"];
    const char* path = config["path"];


    Serial.printf("[OTA] Connecting to %s to fetch http://%s%s ...", ssid, host, path);

    int retries = 10;
    wl_status_t status = WiFi.begin(ssid, pass);
    while (retries > 0 && status != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
        retries--;
        status = WiFi.status();
    }


    ESPhttpUpdate.rebootOnUpdate(false);

    Serial.println("\n[OTA] Updating...");
    auto reply = ESPhttpUpdate.update(host, 80, path); //, "optional current version string here"*/);


    switch (reply)
    {
        case HTTP_UPDATE_FAILED:
            Serial.println("[OTA] Update failed");
            break;

        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("[OTA] Update no updates");
            break;

        case HTTP_UPDATE_OK:
            SPIFFS.end();

            Serial.println("[OTA] Update success.  Rebooting into new firmware...");
            ESP.restart();
            break;
    }

    Serial.println(ESPhttpUpdate.getLastErrorString());
}
