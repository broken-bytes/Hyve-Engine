#pragma once

#ifdef __swift__
#include <stdint.h>
#include <stdlib.h>
#else
#include <cstdint>
#ifdef _WIN32
#include <malloc.h>
#else
#include <stdlib.h>
#endif
#endif

struct SDL_Window;

typedef void* NativeEvent;
typedef SDL_Window* Window;
