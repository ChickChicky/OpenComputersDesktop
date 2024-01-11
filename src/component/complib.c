#include "complib.h"

// C API

comp_slot* comp_get_comp_at(comp_env* env, size_t slot) {
    if (env != NULL) for (size_t i = 0; i < COMP_ENV_N_COMP; i++) {
        comp_slot* s = env->slots[i];
        if (s != NULL && s->slot == slot && s->component != NULL)
            return s;
    }
    return NULL;
}

comp_addr get_comp_addr(comp_env* env, void* comp) {
    if (env != NULL) for (size_t i = 0; i < COMP_ENV_N_COMP; i++) {
        comp_slot* s =  env->slots[i];
        if (s != NULL && s->component == comp)
            return ((comp_base*)s->component)->address;
    }
    return (comp_addr){0,0};
}

void* comp_get_comp_byaddr(comp_env* env, comp_addr addr) {
    if (env != NULL) for (size_t i = 0; i < COMP_ENV_N_COMP; i++) {
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

comp_addr comp_addr_from_string(const char* addr) {
    uint64_t high = 0;
    uint64_t low = 0;
    size_t j = 0;
    for (size_t i = 0; i < 36; i++) {
        if (addr[i] == '-') continue;
        uint64_t h = 0;
        switch (addr[i]) {
            case '0': h = 0x0; break;
            case '1': h = 0x1; break;
            case '2': h = 0x2; break;
            case '3': h = 0x3; break;
            case '4': h = 0x4; break;
            case '5': h = 0x5; break;
            case '6': h = 0x6; break;
            case '7': h = 0x7; break;
            case '8': h = 0x8; break;
            case '9': h = 0x9; break;
            case 'A': h = 0xA; break; case 'a': h = 0xa; break;
            case 'B': h = 0xB; break; case 'b': h = 0xb; break;
            case 'C': h = 0xC; break; case 'c': h = 0xc; break;
            case 'D': h = 0xD; break; case 'd': h = 0xd; break;
            case 'E': h = 0xE; break; case 'e': h = 0xe; break;
            case 'F': h = 0xF; break; case 'f': h = 0xf; break;
            default:
                return (comp_addr){0,0}; // return a NULL component address in case of a non-hex character
        }
        *(j >= 64 ? &low : &high) |= h<<(64-(j%64)); // TODO: make this cleaner :p
        j += 4;
    }
    return (comp_addr){.high=high,.low=low};
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

int comp_env_set(comp_env* env, void* comp, size_t slot) {
    if (env == NULL) return 1;
    size_t idx = SIZE_MAX;
    for (size_t i = 0; i < COMP_ENV_N_COMP; i++) {
        comp_slot* s = env->slots[i];
        if (s != NULL && s->slot == slot) {
            s->component = comp;
            return 1;
        } else if (idx == SIZE_MAX)
            idx = i;
    }
    if (idx == SIZE_MAX) return 1;
    env->slots[idx] = malloc(sizeof(comp_slot));
    env->slots[idx]->slot = slot;
    env->slots[idx]->component = comp;
    return 0;
}

comp_env* comp_env_new() {
    comp_slot** slots = (comp_slot**)malloc(sizeof(comp_slot*)*COMP_ENV_N_COMP);
    comp_env* env = (comp_env*)malloc(sizeof(comp_env));
    env->slots = slots;
    return env;
}

void comp_env_free(comp_env* env) {
    if (env == NULL) return;
    for (size_t i = 0; i < COMP_ENV_N_COMP; i++)
        if (env->slots[i] != NULL) free(env->slots[i]);
    free(env->slots);
    free(env);
}

// Lua API

static int luaB_next (lua_State *L) {
    luaL_checktype(L, 1, LUA_TTABLE);
    printf("SZ: %i\n",lua_gettop(L));
    for (size_t i = 0; i < lua_gettop(L); i++) {
        printf("    %s\n",lua_typename(L, lua_type(L,(i))));
    }
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
    char* addr = (char*)malloc(37);
    if (ENV != NULL) {
        for (size_t i = 0; i < COMP_ENV_N_COMP; i++) {
            comp_slot* slot = ENV->slots[i];
            if (slot != NULL && slot->component != NULL) {
                comp_base* comp = (comp_base*)slot->component;
                comp_addr_str(&comp->address,addr);
                lua_pushstring(L,comp->type);
                lua_setfield(L,-2,addr);
            }
        }
    }
    free(addr);
    return 1;
}

int ocd_L_comp_method_call(lua_State* L) {
    luaL_checktype(L,1,LUA_TTABLE);
    return 0;
}

static const luaL_Reg compMT[] = {
    {NULL,NULL},
};

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
    lua_pop(L,1);

    luaL_newmetatable(L,COMP_COMP_MT);
    luaL_setfuncs(L, compMT, 0);
    lua_pop(L,1);
    
    ocd_open_comp_gpu(L);
    
    return 1;
}