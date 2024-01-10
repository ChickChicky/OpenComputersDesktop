#ifndef componentlib_h
#define componentlib_h

#include "common.h"

/* The amount of slots in the environment */
#define COMP_ENV_N_COMP (1<<16)
/* The name of the component list metatable,
   which simply allows to iterate over the
   result */
#define COMP_COMPLIST_MT "OCD::ComponentList"
/* The name of the component metatable applied
   to all of the components */
#define COMP_COMP_MT "OCD::Component"
/* The name of the metatable applied to the
   component methods */
#define COMP_METHOD_MT "OCD::Component::Method"



struct comp_addr {
    uint64_t low;
    uint64_t high;
};

/* Stores a component address as a 128 bit integer */
typedef struct comp_addr comp_addr;



struct comp_slot {
    size_t slot;
    void* component;
};

/* Stores a reference to a component slot */
typedef struct comp_slot comp_slot;



struct comp_env {
    comp_slot** slots/*[COMP_ENV_N_COMP]*/;
};

/* A component environment (will probably replaced by computer case at some point) */
typedef struct comp_env comp_env;



struct comp_base {
    comp_addr address;
};

/* A structure representing the base data inside all components */
typedef struct comp_base comp_base;



/* Loads the global component lib (with all the single component modules) into the Lua environment */
int ocd_open_complib(lua_State* L);



comp_env* comp_env_new();

comp_addr comp_get_comp_at(comp_env* env, size_t slot);
/* Generates a new random component address */
comp_addr comp_addr_new(void);
/* Writes a component address into a string in the UUID format 8-4-4-4-12 (36 chars) */
void comp_addr_str(comp_addr* addr, char* out);
/* Checks if two component addresses are the same */
int comp_addr_eq(comp_addr* a, comp_addr* b);


#include "gpu.h"

#endif