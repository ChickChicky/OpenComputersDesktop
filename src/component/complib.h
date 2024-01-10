#ifndef componentlib_h
#define componentlib_h

#include "common.h"

#define COMP_COMPLIST_MT "OCD::ComponentList"

struct comp_addr {
    uint64_t low;
    uint64_t high;
};

/* Stores a component address as a 128 bit integer */
typedef struct comp_addr comp_addr;

/* Loads the global component lib (with all the single component modules) into the Lua environment */
int ocd_open_complib(lua_State* L);

/* Generates a new random component address */
comp_addr comp_addr_new(void);
/* Writes a component address into a string in the UUID format 8-4-4-4-12 (36 chars) */
void comp_addr_str(comp_addr* addr, char* out);

#include "gpu.h"

#endif