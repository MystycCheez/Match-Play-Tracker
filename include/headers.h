#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "raylib.h"
#include "GLFW/glfw3.h"

#include "linked_list.h"

#define malloc(X) debug_malloc(X, __FILE__, __LINE__, __FUNCTION__)
#define free(X) debug_free(X)

#include "key.h"

#include "defines.h"
#include "enums.h"
#include "structs.h"

#include "actions.h"
#include "decs.h"

#include "globals.h"

#endif