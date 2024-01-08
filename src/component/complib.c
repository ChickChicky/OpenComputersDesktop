#include "complib.h"

static const luaL_Reg compLib[] = {
    {NULL,NULL}
};

void ocd_complib(lua_State* L) {
    for (const luaL_Reg* lib = compLib; lib->func; lib++) {
        luaL_requiref(L, lib->name, lib->func, 1);
        lua_pop(L, 1);
    }
}