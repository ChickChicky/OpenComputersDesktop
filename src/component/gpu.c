#include "gpu.h"

// Lua API

int ocd_L_gpu_bind(lua_State* L, void* comp) {
    (void)L;
    comp_gpu* gpu = (comp_gpu*)comp;
    printf("BIND FROM %p\n",gpu);
    return 0;
}

comp_method_reg gpu_vtable[] = {
    {"bind",ocd_L_gpu_bind},
    {NULL,NULL},
};

int ocd_open_comp_gpu(lua_State* L) {
    (void)L;
    return 0;
}

// C API

comp_gpu* comp_gpu_new(void) {
    comp_gpu* gpu = (comp_gpu*)malloc(sizeof(comp_gpu));
    gpu->address = comp_addr_new();
    gpu->vtable  = gpu_vtable;
    gpu->type    = "gpu";
    gpu->buffers = malloc(sizeof(gpu_buffer*)*GPU_N_BUFFERS);
    gpu->buffers[0] = gpu_buffer_new(160,50);
    return gpu;
}

void comp_gpu_free(comp_gpu* gpu) {
    if (gpu == NULL) return;
    for (size_t i = 0; i < GPU_N_BUFFERS; i++)
        gpu_buffer_free(gpu->buffers[i]);
    free(gpu->buffers);
    free(gpu);
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
    if (buffer == NULL) return;
    free(buffer->data);
    free(buffer);
}