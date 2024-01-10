#include "complib.h"

// C API

comp_addr comp_get_comp_at(comp_env* env, size_t slot) {
    if (env == NULL) return (comp_addr){0,0};
    for (size_t i = 0; i < COMP_ENV_N_COMP; i++) {
        comp_slot* s = env->slots[i];
        if (s != NULL && s->slot == slot && s->component != NULL)
            return ((comp_base*)s->component)->address;
    }
    return (comp_addr){0,0};
}

void* comp_get_comp_byaddr(comp_env* env, comp_addr addr) {
    if (env == NULL) return NULL;
    for (size_t i = 0; i < COMP_ENV_N_COMP; i++) {
        comp_slot* s = env->slots[i];
        if (s != NULL && s->component != NULL && comp_addr_eq(&addr,&((comp_base*)s->component)->address))
            return s->component;
    }
    return NULL;
}

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

int comp_addr_eq(comp_addr* a, comp_addr* b) {
    return a->high==b->high && a->low==b->low;
}

comp_env* comp_env_new() {
    comp_slot** slots = (comp_slot**)malloc(sizeof(comp_slot*)*COMP_ENV_N_COMP);
    comp_env* env = (comp_env*)malloc(sizeof(comp_env));
    env->slots = slots;
    return env;
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

int ocd_L_comp_list(lua_State* L) {
    lua_newtable(L);
    luaL_getmetatable(L,COMP_COMPLIST_MT);
    lua_setmetatable(L,-2);
    return 1;
}

int ocd_L_comp_method_call(lua_State* L) {
    luaL_checktype(L,1,LUA_TTABLE);
    return 0;
}

static const luaL_Reg compMethodMT[] = {
    {"__call",ocd_L_comp_method_call},
    // {"__tostring",ocd_L_comp_method_tostring},
    {NULL,NULL},
};

static const luaL_Reg compListMT[] = {
    {"__call",luaB_next},
    {NULL,NULL}
};

static const luaL_Reg compLib[] = {
    {"list",ocd_L_comp_list},
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