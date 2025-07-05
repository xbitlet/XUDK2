#include "xudk/uefi.h"

// Storage Management
typedef struct {
    status (*get_disks)(xudk_ctx *ctx, xudk_disk_info **disks, usize *count);
    status (*read_sectors)(xudk_ctx *ctx, u32 disk_id, u64 lba, u32 count, void *buffer);
    status (*write_sectors)(xudk_ctx *ctx, u32 disk_id, u64 lba, u32 count, const void *buffer);
    status (*get_disk_info)(xudk_ctx *ctx, u32 disk_id, xudk_disk_info *info);
    status (*format_disk)(xudk_ctx *ctx, u32 disk_id, const wchar *filesystem);
    status (*create_partition)(xudk_ctx *ctx, u32 disk_id, u64 start, u64 size, u8 type);
} xudk_storage;