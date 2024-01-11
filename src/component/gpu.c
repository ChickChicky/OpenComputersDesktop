#include "gpu.h"

// Lua API

int ocd_L_gpu_bind(lua_State* L, comp_gpu* gpu) {
    printf("BIND FROM %p\n",gpu);
    return 0;
}

comp_method_reg gpu_vtable[] = {
    {"bind",ocd_L_gpu_bind},
    {NULL,NULL},
};

// C API

int ocd_CL_push_gpu(lua_State* L, comp_gpu* gpu) {
    lua_newtable(L);
    luaL_getmetatable(L,COMP_COMP_MT);
    lua_setmetatable(L,-2);
    return 1;
}

int ocd_open_comp_gpu(lua_State* L) {
    luaL_newmetatable(L,COMP_COMP_MT);
    lua_pop(L,1);
    return 0;
}

comp_gpu* comp_gpu_new(void) {
    comp_gpu* gpu = (comp_gpu*)malloc(sizeof(comp_gpu));
    gpu->address = comp_addr_new();
    gpu->vtable  = gpu_vtable;
    gpu->buffers = malloc(sizeof(gpu_buffer*)*GPU_N_BUFFERS);
    return gpu;
}

// GPU

gpu_buffer* gpu_buffer_new(uint16_t width, uint16_t height) {
    gpu_buffer* buff = (gpu_buffer*) malloc(sizeof(gpu_buffer));
    buff->width = width;
    buff->height = height;
    buff->data = malloc(width*height);
    return buff;
}

void gpu_buffer_free(gpu_buffer* buffer) {
    free(buffer->data);
    free(buffer);
}