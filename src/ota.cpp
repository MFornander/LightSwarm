
#include <ArduinoJson.h>
#include <ESP8266httpUpdate.h>
#include <FS.h>

#include "debug.h"
#include "ota.h"

#define CONFIG_NAME "/ota.json"
#define CONFIG_SIZE 200

namespace LightSwarm {

	
#define Serial

OTA::OTA()
{
	SPIFFS.begin();
}


void OTA::RebootUpdate(const String& ssid, const String& pass, const String& host, const String& path)
{
    StaticJsonBuffer<CONFIG_SIZE> jsonBuffer;
    JsonObject& config = jsonBuffer.createObject();

    config["ssid"] = ssid;
    config["pass"] = pass;
    config["host"] = host;
    config["path"] = path;

    SPIFFS.remove(CONFIG_NAME);
    File configFile = SPIFFS.open(CONFIG_NAME, "w");
    if (!configFile)
    {
        ERR("[OTA] Failed to open config file for writing\n");
        return;
    }

    config.printTo(configFile);
    configFile.close();
    SPIFFS.end();

    INFO("[OTA] Rebooting in OTA mode...");
    ESP.restart();
}



void OTA::TryUpdate()
{
    File configFile = SPIFFS.open(CONFIG_NAME, "r");
    if (!configFile)
    {
        INFO("[OTA] No config file. Leaving OTA.\n");
        return;
    }

    size_t size = configFile.size();
    if (size > CONFIG_SIZE)
    {
       ERR("[OTA] Config file size (%d) is too large (%d)\n", (int)size, CONFIG_SIZE);
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
        ERR("[OTA] Failed to parse config file\n");
        return;
    }

    const char* ssid = config["ssid"];
    const char* pass = config["pass"];
    const char* host = config["host"];
    const char* path = config["path"];


    INFO("[OTA] Fetching http://%s%s over ssid=%s...", host, path, ssid);

    int retries = 10;
    wl_status_t status = WiFi.begin(ssid, pass);
    while (retries > 0 && status != WL_CONNECTED)
    {
        delay(1000);
        retries--;
        status = WiFi.status();
    }


    ESPhttpUpdate.rebootOnUpdate(false);

    INFO("\n[OTA] Updating...\n");
    auto reply = ESPhttpUpdate.update(host, 80, path); //, "optional current version string here"*/);


    switch (reply)
    {
        case HTTP_UPDATE_FAILED:
            ERR("[OTA] Update failed\n");
            break;

        case HTTP_UPDATE_NO_UPDATES:
            ERR("[OTA] Update no updates\n");
            break;

        case HTTP_UPDATE_OK:
            INFO("[OTA] Update success.  Rebooting into new firmware...\n");
            break;
    }

    INFO("%s\n", ESPhttpUpdate.getLastErrorString().c_str());
    SPIFFS.end();
    ESP.restart();
}

} // namespace
