#include "lbitlib.h"
#include "ocdlibs.h"

// Defines whether it should also get rid of funtions that are not part
// of OpenComputers but are in Lua 5.4
#define LUA_OC_COMPAT 1

static const luaL_Reg ocdLibs[] = {
    {"component", ocd_open_complib},
    {"bit32", luaopen_bit32},
    {NULL, NULL}  
};

static void ocd_remove_global(lua_State* L, const char* mod) {
    lua_pushnil(L);
    lua_setglobal(L, mod);
    lua_pop(L,-1);
}

static void ocd_remove_field(lua_State* L, const char* mod, const char* name) {
    lua_getglobal(L, mod);
    lua_pushnil(L);
    lua_setfield(L, -2, name);
    lua_pop(L,-1);
}

void ocd_openlibs(lua_State* L) {
    luaL_openlibs(L);
    ocd_open_complib(L);

    for (const luaL_Reg* lib = ocdLibs; lib->func; lib++) {
        luaL_requiref(L, lib->name, lib->func, 1);
        lua_pop(L, 1);
    }

    ocd_remove_global(L,"collectgarbage");
    ocd_remove_global(L,"dofile");
    ocd_remove_global(L,"loadfile");
    ocd_remove_global(L,"warn");
    // ocd_remove_global(L,"print"); // temporarily keep it to debug it more easily

    ocd_remove_global(L,"debug");
    ocd_remove_global(L,"io");
    ocd_remove_global(L,"package");

    ocd_remove_field(L,"os","execute");
    ocd_remove_field(L,"os","exit");
    ocd_remove_field(L,"os","getenv");
    ocd_remove_field(L,"os","remove");
    ocd_remove_field(L,"os","rename");
    ocd_remove_field(L,"os","setlocale");
    ocd_remove_field(L,"os","tmpname");
    
    #if LUA_OC_COMPAT
        ocd_remove_field(L,"table","move");

        ocd_remove_field(L,"math","log10");
        ocd_remove_field(L,"math","maxinteger");
        ocd_remove_field(L,"math","mininteger");
        ocd_remove_field(L,"math","tointeger");
        ocd_remove_field(L,"math","type");
    #endif
}