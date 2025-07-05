#include "xudk/uefi.h"

// System Management
typedef struct {
    status (*get_system_info)(xudk_ctx *ctx, xudk_system_info *info);
    status (*get_time)(xudk_ctx *ctx, xudk_time_info *time);
    status (*set_time)(xudk_ctx *ctx, const xudk_time_info *time);
    status (*reset_system)(xudk_ctx *ctx, u32 reset_type);
    status (*shutdown_system)(xudk_ctx *ctx);
    void   (*delay)(xudk_ctx *ctx, u32 microseconds);
    status (*get_variable)(xudk_ctx *ctx, const wchar *name, const wchar *vendor, void **data, usize *size);
    status (*set_variable)(xudk_ctx *ctx, const wchar *name, const wchar *vendor, const void *data, usize size);
    status (*enable_interrupt)(xudk_ctx *ctx, u32 vector);
    status (*disable_interrupt)(xudk_ctx *ctx, u32 vector);
} xudk_system;
