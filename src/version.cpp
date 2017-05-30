#include "version.h"

#define TEXTIFY(A) #A
#define ESCAPEQUOTE(A) TEXTIFY(A)

namespace LightSwarm {

const char* Version::TAG = ESCAPEQUOTE(VER_TAG);
const char* Version::BUILD = ESCAPEQUOTE(VER_TAG) "-v" ESCAPEQUOTE(VER_MAJOR) "." ESCAPEQUOTE(VER_MINOR);

}
