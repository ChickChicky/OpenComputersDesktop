#ifndef gpu_h
#define gpu_h

#include "common.h"

#define GPU_N_BUFFERS (1<<16)

struct gpu_buffer {
    uint16_t width;
    uint16_t height;
    char* data;
};

/* A buffer used to store scren data on a GPU, see `gpu_buffer_new` */
typedef struct gpu_buffer gpu_buffer;

struct comp_gpu {
    comp_addr address;
    comp_method_reg* vtable;
    const char* type;
    comp_addr bound;
    gpu_buffer** buffers;
};

/* A GPU component that can live in a computer */
typedef struct comp_gpu comp_gpu;

/* Loads the GPU Lua interface */
int ocd_open_comp_gpu(lua_State* L);

/* Creates a new GPU instance */
comp_gpu* comp_gpu_new(void);
/* Frees a previously created GPU */
void comp_gpu_free(comp_gpu* gpu);

/* Allocates a new screen buffer for a GPU */
gpu_buffer* gpu_buffer_new(uint16_t width, uint16_t height);
/* Frees a scren buffer of a GPU */
void gpu_buffer_free(gpu_buffer* buffer);

#endif