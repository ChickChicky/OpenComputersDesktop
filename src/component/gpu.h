#ifndef gpu_h
#define gpu_h

#include "common.h"

#define GPU_N_BUFFERS (1<<16)

struct gpu_buffer {
    uint16_t width;
    uint16_t height;
    char* data;
};

typedef struct gpu_buffer gpu_buffer;

struct comp_gpu {
    comp_addr address;
    comp_addr bound;
    gpu_buffer* buffers[GPU_N_BUFFERS];
};

typedef struct comp_gpu comp_gpu;

int ocd_open_comp_gpu(lua_State* L);

void ocd_push_gpu(lua_State* L, comp_gpu* gpu);

gpu_buffer* gpu_buffer_new(uint16_t width, uint16_t height);
void gpu_buffer_free(gpu_buffer* buffer);

#endif