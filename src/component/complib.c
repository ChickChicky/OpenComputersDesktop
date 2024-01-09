#include "complib.h"

static const luaL_Reg compLib[] = {
    {NULL,NULL}
};

static const char* hexChars = "0123456789ABCDEF";

void ocd_open_complib(lua_State* L) {
    ocd_open_comp_gpu(L);
}

comp_addr comp_addr_new(void) {
    comp_addr addr = {0,0};
    for (size_t i = 0; i < 8; i++) {
        srand(time(0)); addr.low  |= (rand()&255)<<(i*8);
        srand(time(0)); addr.high |= (rand()&255)<<(i*8);
    }
    addr.high &= ~(uint64_t)(7 << 12);
    addr.high |= (uint64_t)(4 << 12);
    return addr;
}

void comp_addr_str(comp_addr* addr, char* out) {
    size_t i = 0;
    for (size_t ci = 0; ci < 36; ci++) {
        if (ci == 8 || ci == 13 || ci == 18 || ci == 23)
            out[ci] = '-';
        else {
            out[ci] = hexChars[((i<16?addr->low:addr->high)>>i)&7];
            i += 4;
        }
    }
}