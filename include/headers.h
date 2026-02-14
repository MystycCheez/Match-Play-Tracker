#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h> // IWYU pragma: keep
#include <stdlib.h>
#include <string.h> // IWYU pragma: keep
#include <assert.h>
#include <math.h> // IWYU pragma: keep

#include "raylib.h" // IWYU pragma: keep
#include "GLFW/glfw3.h" // IWYU pragma: keep

#include "SDL3/SDL.h" // IWYU pragma: keep
#include "SDL3/SDL_main.h" // IWYU pragma: keep

#define malloc(X) debug_malloc(X, __FILE__, __LINE__, __FUNCTION__)
#define free(X) debug_free(X)

#include "key.h" // IWYU pragma: keep

#include "defines.h"
#include "enums.h" // IWYU pragma: keep
#include "structs.h" // IWYU pragma: keep

#include "actions.h" // IWYU pragma: keep
#include "decs.h"

#include "globals.h" // IWYU pragma: keep

#endif