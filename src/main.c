#include "common.h"

#include "ocdlibs.h"
#include "raylib.h"

comp_env* ENV = NULL;

int main(int argc, char** argv) {
    (void) argc; (void) argv;

    ENV = comp_env_new();

    lua_State* L = luaL_newstate();
    ocd_openlibs(L);
    int err = luaL_dofile(L, "script.lua");

    if (err) {
        printf("\x1b[31mFATAL ERROR:\x1b[39m\n%s\n",lua_tolstring(L, -1, NULL));
        lua_pop(L, 1);
    }

    comp_gpu* gpu = comp_gpu_new();

    return 0;
}