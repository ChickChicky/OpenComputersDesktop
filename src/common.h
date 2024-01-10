#ifndef common_h
#define common_h

typedef struct comp_env comp_env;

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <malloc.h>
#include <time.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "complib.h"

extern comp_env* ENV;

#endif