#pragma once

#ifdef WIN32
#include <string.h>

#define krkr_strcasecmp _stricmp

#else // WIN32
#include <strings.h>

#define krkr_strcasecmp strcasecmp
#endif // WIN32

