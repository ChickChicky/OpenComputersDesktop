#include "gpu.h"

int ocd_push_gpu(lua_State* L, comp_gpu* gpu) {
    comp_gpu** lua_gpu = (comp_gpu**)lua_newuserdata(L,sizeof(comp_gpu*));
    *lua_gpu = gpu;
    luaL_getmetatable(L,COMP_GPU_MT);
    lua_setmetatable(L,-2);
    return 1;
}

int ocd_open_comp_gpu(lua_State* L) {
    luaL_newmetatable(L,COMP_GPU_MT);
    lua_pop(L,-1);
    return 0;
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