#include "complib.h"

// C API

static const char* hexChars = "0123456789abcdef";

comp_addr comp_addr_new(void) {
    comp_addr addr = {0,0};
    unsigned int seed = (unsigned int)time(0);
    unsigned int a;
    for (size_t i = 0; i < 8; i++) {
        srand((unsigned int)seed);
        a = rand();
        srand(seed^a^(unsigned int)time(0)); seed = rand();
        addr.low  ^= (uint64_t)a<<(i*8);
        addr.high ^= (uint64_t)seed<<(i*8);
    }
    addr.high &= ~(uint64_t)(15 << 16);
    addr.high |=  (uint64_t)(4  << 16);
    return addr;
}

void comp_addr_str(comp_addr* addr, char* out) {
    size_t i = 0;
    for (size_t ci = 0; ci < 36; ci++) {
        if (ci == 8 || ci == 13 || ci == 18 || ci == 23)
            out[ci] = '-';
        else {
            out[ci] = hexChars[((i<64?addr->high:addr->low)>>(64-(i%64)))&15];
            i += 4;
        }
    }
}

// Lua API

static int luaB_next (lua_State *L) {
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_settop(L, 2);  /* create a 2nd argument if there isn't one */
    if (lua_next(L, 1))
        return 2;
    else {
        lua_pushnil(L);
        return 1;
    }
}

static const luaL_Reg compListMT[] = {
    {"__call",luaB_next},
    {NULL,NULL}
};

int ocd_comp_list(lua_State* L) {
    lua_newtable(L);
    lua_getmetatable(L,COMP_COMPLIST_MT);
    lua_setmetatable(L,-2);
    return 1;
}

static const luaL_Reg compLib[] = {
    {"list",ocd_comp_list},
    {NULL,NULL}
};

int ocd_open_complib(lua_State* L) {
    luaL_newlib(L, compLib);
    luaL_newmetatable(L,COMP_COMPLIST_MT);
    luaL_setfuncs(L, compListMT, 0);
    ocd_open_comp_gpu(L);
    /* TODO: probably have to pop the new metatable */
    return 1;
}