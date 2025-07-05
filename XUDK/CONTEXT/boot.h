#include "xudk/uefi.h"


// Boot Management
typedef struct {
    status (*get_boot_entries)(xudk_ctx *ctx, xudk_boot_entry **entries, usize *count);
    status (*create_boot_entry)(xudk_ctx *ctx, const xudk_boot_entry *entry, u16 *boot_number);
    status (*delete_boot_entry)(xudk_ctx *ctx, u16 boot_number);
    status (*set_boot_order)(xudk_ctx *ctx, u16 *boot_order, usize count);
    status (*get_boot_order)(xudk_ctx *ctx, u16 **boot_order, usize *count);
    status (*set_boot_next)(xudk_ctx *ctx, u16 boot_number);
    status (*load_image)(xudk_ctx *ctx, const wchar *path, handle *image);
    status (*start_image)(xudk_ctx *ctx, handle image, usize *exit_data_size, wchar **exit_data);
    status (*exit_boot_services)(xudk_ctx *ctx, handle image, usize map_key);
} xudk_boot;