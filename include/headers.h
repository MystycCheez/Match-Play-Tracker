#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "raylib.h"
#include "GLFW/glfw3.h"

#define malloc(X) my_malloc( X, __FILE__, __LINE__, __FUNCTION__)

#include "key.h"

#include "defines.h"
#include "enums.h"
#include "structs.h"

#include "actions.h"
#include "decs.h"

#include "globals.h"

#endif