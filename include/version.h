#pragma once
#include <Arduino.h>

namespace LightSwarm {

#define TEXTIFY(A) #A
#define ESCAPEQUOTE(A) TEXTIFY(A)

class Version
{
public:
    static constexpr const char* BUILD = "v" ESCAPEQUOTE(VER_MAJOR) "." ESCAPEQUOTE(VER_MINOR) "-" ESCAPEQUOTE(GIT_REV);
    static constexpr uint32_t MAJOR = VER_MAJOR;
    static constexpr uint32_t MINOR = VER_MINOR;
};
} // namespace

