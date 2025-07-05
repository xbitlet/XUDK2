#include "xudk/uefi.h"


// Network Management
typedef struct {
    status (*get_interfaces)(xudk_ctx *ctx, xudk_net_info **interfaces, usize *count);
    status (*configure_interface)(xudk_ctx *ctx, u32 interface_id, const xudk_net_info *config);
    status (*send_packet)(xudk_ctx *ctx, u32 interface_id, const void *packet, usize size);
    status (*receive_packet)(xudk_ctx *ctx, u32 interface_id, void *buffer, usize buffer_size, usize *received);
    status (*resolve_hostname)(xudk_ctx *ctx, const char *hostname, u32 *ip_address);
    status (*download_file)(xudk_ctx *ctx, const char *url, const wchar *local_path);
} xudk_network;
