#pragma once
#include <Arduino.h>

namespace LightSwarm {

class OTA final
{
public:
	OTA();

	/// Reboot in OTA mode to load the indicated firmware
	void RebootUpdate(const String& ssid, const String& pass, const String& host, const String& uri);


	/// Give OTA a chance to update at every startup
	void TryUpdate();
};
}
