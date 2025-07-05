/*
 * XUDK - Xbitlet UEFI Development Kit
 * Professional UEFI Development Framework for Complex Applications
 * Designed for: Bootloaders, Firmware Tools, System Utilities
 * Version 2.1 - Now with GPU Acceleration Support
 */

#ifndef XUDK_H
#define XUDK_H

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#include "gpu.h"
#include "dat.h"
#include "context/console.h"
#include "context/inp.h"
#include "context/mem.h"
#include "context/fsmng.h"
#include "context/strg.h"
#include "context/boot.h"
#include "context/sys.h"
#include "context/net.h"
#include "context/ghc.h"

#include "xudk.h"

// =============================================================================
// XUDK CONTEXT - Professional Development Interface
// =============================================================================


// Main XUDK Context
struct xudk_ctx {
    // Management subsystems
    xudk_console    console;
    xudk_input      input;
    xudk_memory     memory;
    xudk_filesystem filesystem;
    xudk_storage    storage;
    xudk_boot       boot;
    xudk_system     system;
    xudk_network    network;
    xudk_graphics   graphics;
    xudk_gpu        gpu;        // New GPU subsystem
    
    // Internal state
    handle          image_handle;
    void*           system_table;
    void*           boot_services;
    void*           runtime_services;
    bool            boot_services_active;
    u32             debug_level;
    
    // GPU state
    bool            gpu_initialized;
    u32             active_gpu_device;
    xudk_gpu_info   gpu_device_info;
    xudk_gpu_heap*  gpu_memory_heaps;
    usize           gpu_heap_count;
    
    // Error handling
    status          last_error;
    wchar*          last_error_message;
    void            (*error_handler)(xudk_ctx *ctx, status error, const wchar *message);
    
    // Event system
    void            (*event_handler)(xudk_ctx *ctx, u32 event_type, void *event_data);
    
    // Memory pools for automatic cleanup
    void**          allocated_pools;
    usize           pool_count;
    usize           pool_capacity;
};

// =============================================================================
// INITIALIZATION & MANAGEMENT
// =============================================================================

// Initialize XUDK context
status xudk_init(xudk_ctx *ctx, handle image_handle, void *system_table);

// Cleanup XUDK context
void xudk_cleanup(xudk_ctx *ctx);

// Set debug level (0=none, 1=basic, 2=verbose, 3=detailed)
void xudk_set_debug_level(xudk_ctx *ctx, u32 level);

// Set custom error handler
void xudk_set_error_handler(xudk_ctx *ctx, void (*handler)(xudk_ctx *ctx, status error, const wchar *message));

// Set custom event handler
void xudk_set_event_handler(xudk_ctx *ctx, void (*handler)(xudk_ctx *ctx, u32 event_type, void *event_data));

// =============================================================================
// GPU HELPER FUNCTIONS
// =============================================================================

// GPU initialization and detection
status xudk_gpu_auto_init(xudk_ctx *ctx);
status xudk_gpu_init_best_device(xudk_ctx *ctx);
const wchar* xudk_gpu_vendor_name(xudk_gpu_vendor vendor);
const wchar* xudk_gpu_format_name(xudk_texture_format format);

// GPU resource helpers
status xudk_gpu_create_vertex_buffer(xudk_ctx *ctx, const void *vertices, usize size, xudk_gpu_buffer *buffer);
status xudk_gpu_create_index_buffer(xudk_ctx *ctx, const void *indices, usize size, bool is_32bit, xudk_gpu_buffer *buffer);
status xudk_gpu_create_uniform_buffer(xudk_ctx *ctx, usize size, xudk_gpu_buffer *buffer);
status xudk_gpu_update_uniform_buffer(xudk_ctx *ctx, xudk_gpu_buffer *buffer, const void *data, usize size);

// Common shader creation helpers
status xudk_gpu_create_vertex_shader_from_file(xudk_ctx *ctx, const wchar *path, xudk_gpu_shader *shader);
status xudk_gpu_create_fragment_shader_from_file(xudk_ctx *ctx, const wchar *path, xudk_gpu_shader *shader);
status xudk_gpu_create_compute_shader_from_file(xudk_ctx *ctx, const wchar *path, xudk_gpu_shader *shader);

// Pipeline creation helpers
status xudk_gpu_create_simple_pipeline(xudk_ctx *ctx, xudk_gpu_shader *vs, xudk_gpu_shader *fs, 
                                       xudk_primitive_topology topology, xudk_gpu_pipeline *pipeline);
status xudk_gpu_create_fullscreen_pipeline(xudk_ctx *ctx, xudk_gpu_shader *fs, xudk_gpu_pipeline *pipeline);

// Render pass helpers
status xudk_gpu_create_simple_render_pass(xudk_ctx *ctx, xudk_gpu_texture *color_target, 
                                         xudk_gpu_texture *depth_target, xudk_gpu_render_pass *render_pass);
status xudk_gpu_create_offscreen_render_pass(xudk_ctx *ctx, u32 width, u32 height, 
                                            xudk_texture_format format, xudk_gpu_render_pass *render_pass);

// Drawing helpers
status xudk_gpu_draw_fullscreen_quad(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer);
status xudk_gpu_draw_triangle(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer, 
                             float x1, float y1, float x2, float y2, float x3, float y3, u32 color);
status xudk_gpu_draw_rectangle(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer, 
                              float x, float y, float width, float height, u32 color);
status xudk_gpu_draw_textured_quad(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer, 
                                  xudk_gpu_texture *texture, float x, float y, float width, float height);

// Compute helpers
status xudk_gpu_dispatch_compute(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer, 
                                xudk_gpu_pipeline *pipeline, u32 width, u32 height, u32 depth);

// =============================================================================
// UTILITY FUNCTIONS
// =============================================================================

// Error handling
#define xudk_ok(s)           ((s) == XUDK_OK)
#define xudk_error(s)        ((s) != XUDK_OK)
#define xudk_check(ctx, expr) do { status _s = (expr); if (xudk_error(_s)) { xudk_handle_error(ctx, _s, L#expr); return _s; } } while(0)
#define xudk_assert(ctx, expr) do { if (!(expr)) { xudk_handle_error(ctx, XUDK_ERROR, L"Assertion failed: " L#expr); return XUDK_ERROR; } } while(0)

// GPU-specific error handling
#define xudk_gpu_check(ctx, expr) do { status _s = (expr); if (xudk_error(_s)) { xudk_handle_error(ctx, _s, L"GPU Error: " L#expr); return _s; } } while(0)

// String utilities
usize  xudk_strlen(const wchar *str);
void   xudk_strcpy(wchar *dst, const wchar *src);
int    xudk_strcmp(const wchar *s1, const wchar *s2);
wchar* xudk_strdup(xudk_ctx *ctx, const wchar *src);
void   xudk_ascii_to_unicode(const char *ascii, wchar *unicode);
void   xudk_unicode_to_ascii(const wchar *unicode, char *ascii);

// Memory utilities
void   xudk_memset(void *ptr, u8 value, usize size);
void   xudk_memcpy(void *dst, const void *src, usize size);
int    xudk_memcmp(const void *s1, const void *s2, usize size);
void*  xudk_memalloc_tracked(xudk_ctx *ctx, usize size);  // Automatically tracked for cleanup

// Math utilities
u64    xudk_align_up(u64 value, u64 alignment);
u64    xudk_align_down(u64 value, u64 alignment);
u32    xudk_crc32(const void *data, usize size);
u64    xudk_hash64(const void *data, usize size);

// GPU math utilities
void   xudk_mat4_identity(float mat[16]);
void   xudk_mat4_perspective(float mat[16], float fov, float aspect, float near, float far);
void   xudk_mat4_ortho(float mat[16], float left, float right, float bottom, float top, float near, float far);
void   xudk_mat4_look_at(float mat[16], float eye[3], float center[3], float up[3]);
void   xudk_mat4_translate(float mat[16], float x, float y, float z);
void   xudk_mat4_rotate(float mat[16], float angle, float x, float y, float z);
void   xudk_mat4_scale(float mat[16], float x, float y, float z);
void   xudk_mat4_multiply(float result[16], const float a[16], const float b[16]);
void   xudk_vec3_normalize(float vec[3]);
void   xudk_vec3_cross(float result[3], const float a[3], const float b[3]);
float  xudk_vec3_dot(const float a[3], const float b[3]);

// Configuration management
status xudk_load_config(xudk_ctx *ctx, const wchar *path);
status xudk_save_config(xudk_ctx *ctx, const wchar *path);
status xudk_get_config_value(xudk_ctx *ctx, const wchar *key, wchar *value, usize value_size);
status xudk_set_config_value(xudk_ctx *ctx, const wchar *key, const wchar *value);

// Logging system
void   xudk_log_info(xudk_ctx *ctx, const wchar *format, ...);
void   xudk_log_warning(xudk_ctx *ctx, const wchar *format, ...);
void   xudk_log_error(xudk_ctx *ctx, const wchar *format, ...);
void   xudk_log_debug(xudk_ctx *ctx, const wchar *format, ...);

// Internal error handling
void   xudk_handle_error(xudk_ctx *ctx, status error, const wchar *message);

// =============================================================================
// BOOTLOADER SPECIFIC FUNCTIONS
// =============================================================================

// Kernel loading utilities
status xudk_load_kernel(xudk_ctx *ctx, const wchar *path, addr *entry_point, u64 *size);
status xudk_load_initrd(xudk_ctx *ctx, const wchar *path, addr *base, u64 *size);
status xudk_setup_paging(xudk_ctx *ctx, addr kernel_base, u64 kernel_size);
status xudk_jump_to_kernel(xudk_ctx *ctx, addr entry_point, void *boot_params);

// Boot parameter management
status xudk_create_boot_params(xudk_ctx *ctx, void **params, usize *size);
status xudk_add_boot_param(xudk_ctx *ctx, void *params, const wchar *key, const wchar *value);
status xudk_set_kernel_cmdline(xudk_ctx *ctx, void *params, const wchar *cmdline);

// Hardware detection
status xudk_detect_hardware(xudk_ctx *ctx);
status xudk_get_acpi_tables(xudk_ctx *ctx, void **tables, usize *count);
status xudk_get_smbios_data(xudk_ctx *ctx, void **data, usize *size);

// =============================================================================
// MAIN ENTRY POINT
// =============================================================================

// Define this function in your application
status xudk_main(xudk_ctx *ctx);

// Standard UEFI entry point (implemented in XUDK)
status efi_main(handle image_handle, void *system_table);

// =============================================================================
// GPU SHADER EXAMPLES
// =============================================================================

// Simple vertex shader source (HLSL-style)
extern const char* xudk_basic_vertex_shader_source;
/*
static const char* xudk_basic_vertex_shader_source = 
"cbuffer VertexBuffer : register(b0) {\n"
"    float4x4 mvp_matrix;\n"
"};\n"
"\n"
"struct VS_INPUT {\n"
"    float3 position : POSITION;\n"
"    float2 texcoord : TEXCOORD0;\n"
"    float4 color : COLOR;\n"
"};\n"
"\n"
"struct VS_OUTPUT {\n"
"    float4 position : SV_POSITION;\n"
"    float2 texcoord : TEXCOORD0;\n"
"    float4 color : COLOR;\n"
"};\n"
"\n"
"VS_OUTPUT main(VS_INPUT input) {\n"
"    VS_OUTPUT output;\n"
"    output.position = mul(float4(input.position, 1.0), mvp_matrix);\n"
"    output.texcoord = input.texcoord;\n"
"    output.color = input.color;\n"
"    return output;\n"
"}\n";
*/

// Simple fragment shader source
extern const char* xudk_basic_fragment_shader_source;
/*
static const char* xudk_basic_fragment_shader_source = 
"Texture2D diffuse_texture : register(t0);\n"
"SamplerState texture_sampler : register(s0);\n"
"\n"
"struct PS_INPUT {\n"
"    float4 position : SV_POSITION;\n"
"    float2 texcoord : TEXCOORD0;\n"
"    float4 color : COLOR;\n"
"};\n"
"\n"
"float4 main(PS_INPUT input) : SV_TARGET {\n"
"    float4 texture_color = diffuse_texture.Sample(texture_sampler, input.texcoord);\n"
"    return texture_color * input.color;\n"
"}\n";
*/

// =============================================================================
// EXAMPLE USAGE - GPU ACCELERATED BOOTLOADER
// =============================================================================

/*
// Example: GPU-accelerated bootloader with fancy graphics

#include "xudk.h"

// Vertex structure for our boot menu
typedef struct {
    float position[3];
    float texcoord[2];
    u32   color;
} boot_vertex;

status xudk_main(xudk_ctx *ctx) {
    xudk_system_info sys_info;
    xudk_boot_entry *entries;
    usize entry_count;
    
    // Initialize console
    ctx->console.clear(ctx);
    ctx->console.set_color(ctx, 15, 0);  // White on black
    ctx->console.println(ctx, L"XUDK GPU Bootloader v2.1");
    
    // Try to initialize GPU
    if (xudk_ok(xudk_gpu_auto_init(ctx))) {
        ctx->console.println(ctx, L"GPU acceleration enabled!");
        
        // GPU-accelerated boot menu
        return xudk_gpu_boot_menu(ctx);
    } else {
        ctx->console.println(ctx, L"GPU not available, using software rendering");
        
        // Fallback to software rendering
        return xudk_software_boot_menu(ctx);
    }
}

status xudk_gpu_boot_menu(xudk_ctx *ctx) {
    // Create GPU resources
    xudk_gpu_buffer vertex_buffer, index_buffer, uniform_buffer;
    xudk_gpu_texture background_texture, logo_texture;
    xudk_gpu_shader vertex_shader, fragment_shader;
    xudk_gpu_pipeline pipeline;
    xudk_gpu_cmd_buffer cmd_buffer;
    xudk_gpu_render_pass render_pass;
    
    // Load shaders
    xudk_gpu_check(ctx, xudk_gpu_create_vertex_shader_from_file(ctx, L"\\EFI\\BOOT\\shaders\\menu.vs", &vertex_shader));
    xudk_gpu_check(ctx, xudk_gpu_create_fragment_shader_from_file(ctx, L"\\EFI\\BOOT\\shaders\\menu.fs", &fragment_shader));
    
    // Create pipeline
    xudk_gpu_check(ctx, xudk_gpu_create_simple_pipeline(ctx, &vertex_shader, &fragment_shader, 
                                                       XUDK_TOPOLOGY_TRIANGLES, &pipeline));
    
    // Create vertex buffer for menu quad
    boot_vertex vertices[] = {
        {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}, 0xFFFFFFFF},
        {{ 1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}, 0xFFFFFFFF},
        {{ 1.0f,  1.0f, 0.0f}, {1.0f, 0.0f}, 0xFFFFFFFF},
        {{-1.0f,  1.0f, 0.0f}, {0.0f, 0.0f}, 0xFFFFFFFF}
    };
    
    u16 indices[] = {0, 1, 2, 0, 2, 3};
    
    xudk_gpu_check(ctx, xudk_gpu_create_vertex_buffer(ctx, vertices, sizeof(vertices), &vertex_buffer));
    xudk_gpu_check(ctx, xudk_gpu_create_index_buffer(ctx, indices, sizeof(indices), false, &index_buffer));
    xudk_gpu_check(ctx, xudk_gpu_create_uniform_buffer(ctx, sizeof(float) * 16, &uniform_buffer));
    
    // Load background texture
    xudk_gpu_check(ctx, ctx->gpu.load_texture_from_file(ctx, L"\\EFI\\BOOT\\images\\background.png", &background_texture));
    
    // Create render pass
    xudk_gpu_texture *color_targets[] = {&background_texture};
    render_pass.color_targets = color_targets;
    render_pass.color_target_count = 1;
    render_pass.depth_target = null;
    render_pass.width = 1920;
    render_pass.height = 1080;
    render_pass.clear_color = true;
    render_pass.clear_depth = false;
    render_pass.clear_color_value[0] = 0.0f;
    render_pass.clear_color_value[1] = 0.0f;
    render_pass.clear_color_value[2] = 0.2f;
    render_pass.clear_color_value[3] = 1.0f;
    
    // Create command buffer
    xudk_gpu_check(ctx, ctx->gpu.create_command_buffer(ctx, false, &cmd_buffer));
    
    // Set up MVP matrix
    float mvp_matrix[16];
    xudk_mat4_identity(mvp_matrix);
    xudk_gpu_check(ctx, xudk_gpu_update_uniform_buffer(ctx, &uniform_buffer, mvp_matrix, sizeof(mvp_matrix)));
    
    // Main render loop
    xudk_key_input key;
    bool running = true;
    
    while (running) {
        // Begin command buffer recording
        xudk_gpu_check(ctx, ctx->gpu.begin_recording(ctx, &cmd_buffer));
        
        // Begin render pass
        xudk_gpu_check(ctx, ctx->gpu.begin_render_pass(ctx, &cmd_buffer, &render_pass));
        
        // Set viewport
        xudk_gpu_check(ctx, ctx->gpu.set_viewport(ctx, &cmd_buffer, 0, 0, 1920, 1080));
        
        // Bind pipeline and resources
        xudk_gpu_check(ctx, ctx->gpu.bind_pipeline(ctx, &cmd_buffer, &pipeline));
        xudk_gpu_check(ctx, ctx->gpu.bind_vertex_buffers(ctx, &cmd_buffer, 0, 1, &vertex_buffer, null));
        xudk_gpu_check(ctx, ctx->gpu.bind_index_buffer(ctx, &cmd_buffer, &index_buffer, 0, false));
        
        // Draw the menu background
        xudk_gpu_check(ctx, ctx->gpu.draw_indexed(ctx, &cmd_buffer, 6, 1, 0, 0, 0));
        
        // End render pass
        xudk_gpu_check(ctx, ctx->gpu.end_render_pass(ctx, &cmd_buffer));
        
        // End command buffer recording
        xudk_gpu_check(ctx, ctx->gpu.end_recording(ctx, &cmd_buffer));
        
        // Submit and present
        xudk_gpu_check(ctx, ctx->gpu.submit_command_buffer(ctx, &cmd_buffer));
        xudk_gpu_check(ctx, ctx->gpu.present_to_screen(ctx, &background_texture));
        
        // Handle input
        if (ctx->input.key_available(ctx)) {
            ctx->input.read_key(ctx, &key);
            
            if (key.scan_code == 0x17) { // ESC
                running = false;
            }
        }
        
        // Small delay
        ctx->system.delay(ctx, 16000); // ~60 FPS
    }
    
    // Cleanup
    ctx->gpu.destroy_command_buffer(ctx, &cmd_buffer);
    ctx->gpu.destroy_pipeline(ctx, &pipeline);
    ctx->gpu.destroy_shader(ctx, &vertex_shader);
    ctx->gpu.destroy_shader(ctx, &fragment_shader);
    ctx->gpu.free_buffer(ctx, &vertex_buffer);
    ctx->gpu.free_buffer(ctx, &index_buffer);
    ctx->gpu.free_buffer(ctx, &uniform_buffer);
    ctx->gpu.destroy_texture(ctx, &background_texture);
    
    return XUDK_OK;
}

status xudk_software_boot_menu(xudk_ctx *ctx) {
    // Fallback software implementation
    xudk_system_info sys_info;
    xudk_boot_entry *entries;
    usize entry_count;
    
    // Get system information
    if (xudk_ok(ctx->system.get_system_info(ctx, &sys_info))) {
        ctx->console.print(ctx, L"System: %s %s\n", sys_info.system_vendor, sys_info.product_name);
        ctx->console.print(ctx, L"Memory: %llu MB\n", sys_info.total_memory / 1024 / 1024);
    }
    
    // Load configuration
    xudk_load_config(ctx, L"\\EFI\\BOOT\\bootloader.conf");
    
    // Get boot entries
    if (xudk_ok(ctx->boot.get_boot_entries(ctx, &entries, &entry_count))) {
        ctx->console.println(ctx, L"\nAvailable boot entries:");
        for (usize i = 0; i < entry_count; i++) {
            ctx->console.print(ctx, L"%d. %s\n", i + 1, entries[i].description);
        }
    }
    
    // Boot menu
    xudk_key_input key;
    ctx->console.println(ctx, L"\nSelect boot entry (1-9) or ESC to exit:");
    
    while (true) {
        ctx->input.wait_key(ctx, &key);
        
        if (key.scan_code == 0x17) { // ESC
            break;
        }
        
        if (key.unicode >= L'1' && key.unicode <= L'9') {
            u32 selection = key.unicode - L'1';
            if (selection < entry_count) {
                ctx->console.print(ctx, L"Booting: %s\n", entries[selection].description);
                
                // Load and start the selected boot entry
                handle image;
                if (xudk_ok(ctx->boot.load_image(ctx, entries[selection].file_path, &image))) {
                    ctx->boot.start_image(ctx, image, null, null);
                }
                break;
            }
        }
    }
    
    return XUDK_OK;
}
*/

#endif // XUDK_H