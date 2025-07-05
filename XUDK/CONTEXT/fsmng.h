#include "xudk/uefi.h"

// File System Management
typedef struct {
    status (*mount_volume)(xudk_ctx *ctx, handle device, xudk_volume_info *info);
    status (*unmount_volume)(xudk_ctx *ctx, handle volume);
    status (*open_file)(xudk_ctx *ctx, const wchar *path, handle *file);
    status (*close_file)(xudk_ctx *ctx, handle file);
    status (*read_file)(xudk_ctx *ctx, handle file, void *buffer, usize size, usize *read);
    status (*write_file)(xudk_ctx *ctx, handle file, const void *buffer, usize size, usize *written);
    status (*get_file_size)(xudk_ctx *ctx, handle file, u64 *size);
    status (*list_directory)(xudk_ctx *ctx, const wchar *path, wchar ***entries, usize *count);
    status (*get_volumes)(xudk_ctx *ctx, xudk_volume_info **volumes, usize *count);
    status (*load_file_to_memory)(xudk_ctx *ctx, const wchar *path, void **buffer, usize *size);
} xudk_filesystem;