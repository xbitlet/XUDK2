#include "xudk/uefi.h"

// Memory Management
typedef struct {
    void*  (*alloc)(xudk_ctx *ctx, usize size);
    void*  (*alloc_aligned)(xudk_ctx *ctx, usize size, usize alignment);
    void*  (*alloc_type)(xudk_ctx *ctx, usize size, xudk_mem_type type);
    void   (*free)(xudk_ctx *ctx, void *ptr);
    status (*get_memory_map)(xudk_ctx *ctx, xudk_mem_map *map);
    void   (*free_memory_map)(xudk_ctx *ctx, xudk_mem_map *map);
    status (*set_virtual_map)(xudk_ctx *ctx, xudk_mem_map *map);
    u64    (*get_total_memory)(xudk_ctx *ctx);
    u64    (*get_free_memory)(xudk_ctx *ctx);
} xudk_memory;