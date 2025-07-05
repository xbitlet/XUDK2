#include "xudk/uefi.h"


// Input Management
typedef struct {
    status (*read_key)(xudk_ctx *ctx, xudk_key_input *key);
    status (*wait_key)(xudk_ctx *ctx, xudk_key_input *key);
    status (*flush_input)(xudk_ctx *ctx);
    status (*set_repeat_delay)(xudk_ctx *ctx, u32 delay);
    bool   (*key_available)(xudk_ctx *ctx);
} xudk_input;