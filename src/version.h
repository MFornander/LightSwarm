#pragma once

#include <Arduino.h>

namespace LightSwarm {

class Version
{
public:
	static const char*    TAG;
	static const char*    BUILD;
	static const uint32_t MAJOR = VER_MAJOR;
	static const uint32_t MINOR = VER_MINOR;
};
} // namespace

