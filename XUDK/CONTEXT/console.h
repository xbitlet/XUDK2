#include "xudk/uefi.h"

typedef struct xudk_ctx xudk_ctx;

// Console Management
typedef struct {
    status (*print)(xudk_ctx *ctx, const wchar *fmt, ...);
    status (*println)(xudk_ctx *ctx, const wchar *text);
    status (*clear)(xudk_ctx *ctx);
    status (*set_color)(xudk_ctx *ctx, u8 fg, u8 bg);
    status (*set_cursor)(xudk_ctx *ctx, u32 x, u32 y);
    status (*get_cursor)(xudk_ctx *ctx, u32 *x, u32 *y);
    status (*set_mode)(xudk_ctx *ctx, u32 mode);
    status (*get_mode_info)(xudk_ctx *ctx, u32 mode, xudk_graphics_mode *info);
} xudk_console;
