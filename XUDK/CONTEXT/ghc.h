#include "xudk/uefi.h"

// Graphics Management (CPU-based)
typedef struct {
    status (*get_modes)(xudk_ctx *ctx, xudk_graphics_mode **modes, usize *count);
    status (*set_mode)(xudk_ctx *ctx, u32 mode_number);
    status (*get_framebuffer)(xudk_ctx *ctx, addr *base, usize *size);
    status (*draw_pixel)(xudk_ctx *ctx, u32 x, u32 y, u32 color);
    status (*draw_rectangle)(xudk_ctx *ctx, u32 x, u32 y, u32 width, u32 height, u32 color);
    status (*draw_text)(xudk_ctx *ctx, u32 x, u32 y, const wchar *text, u32 color);
    status (*load_bitmap)(xudk_ctx *ctx, const wchar *path, u32 x, u32 y);
    status (*copy_buffer)(xudk_ctx *ctx, const void *buffer, u32 x, u32 y, u32 width, u32 height);
} xudk_graphics;

// GPU Management - Hardware Accelerated Graphics
typedef struct {
    // Device Management
    status (*enumerate_devices)(xudk_ctx *ctx, xudk_gpu_info **devices, usize *count);
    status (*initialize_device)(xudk_ctx *ctx, u32 device_index);
    status (*shutdown_device)(xudk_ctx *ctx);
    status (*get_device_info)(xudk_ctx *ctx, xudk_gpu_info *info);
    status (*get_memory_heaps)(xudk_ctx *ctx, xudk_gpu_heap **heaps, usize *count);
    
    // Memory Management
    status (*allocate_buffer)(xudk_ctx *ctx, u64 size, xudk_buffer_usage usage, xudk_gpu_buffer *buffer);
    status (*free_buffer)(xudk_ctx *ctx, xudk_gpu_buffer *buffer);
    status (*map_buffer)(xudk_ctx *ctx, xudk_gpu_buffer *buffer, void **mapped_ptr);
    status (*unmap_buffer)(xudk_ctx *ctx, xudk_gpu_buffer *buffer);
    status (*upload_buffer_data)(xudk_ctx *ctx, xudk_gpu_buffer *buffer, const void *data, usize size, usize offset);
    
    // Texture Management
    status (*create_texture)(xudk_ctx *ctx, u32 width, u32 height, u32 depth, xudk_texture_format format, 
                            u32 mip_levels, u32 array_size, xudk_gpu_texture *texture);
    status (*create_render_target)(xudk_ctx *ctx, u32 width, u32 height, xudk_texture_format format, 
                                  u32 sample_count, xudk_gpu_texture *texture);
    status (*create_depth_stencil)(xudk_ctx *ctx, u32 width, u32 height, xudk_texture_format format, 
                                  u32 sample_count, xudk_gpu_texture *texture);
    status (*destroy_texture)(xudk_ctx *ctx, xudk_gpu_texture *texture);
    status (*upload_texture_data)(xudk_ctx *ctx, xudk_gpu_texture *texture, const void *data, usize size, 
                                 u32 mip_level, u32 array_slice);
    status (*load_texture_from_file)(xudk_ctx *ctx, const wchar *path, xudk_gpu_texture *texture);
    
    // Shader Management
    status (*create_shader)(xudk_ctx *ctx, xudk_shader_type type, const void *bytecode, usize size, 
                           const wchar *entry_point, xudk_gpu_shader *shader);
    status (*compile_shader)(xudk_ctx *ctx, xudk_shader_type type, const char *source, const char *entry_point,
                            xudk_gpu_shader *shader);
    status (*destroy_shader)(xudk_ctx *ctx, xudk_gpu_shader *shader);
    status (*load_shader_from_file)(xudk_ctx *ctx, const wchar *path, xudk_shader_type type, 
                                   const wchar *entry_point, xudk_gpu_shader *shader);
    
    // Pipeline Management
    status (*create_graphics_pipeline)(xudk_ctx *ctx, const xudk_gpu_pipeline_desc *desc, xudk_gpu_pipeline *pipeline);
    status (*create_compute_pipeline)(xudk_ctx *ctx, xudk_gpu_shader *compute_shader, xudk_gpu_pipeline *pipeline);
    status (*destroy_pipeline)(xudk_ctx *ctx, xudk_gpu_pipeline *pipeline);
    
    // Command Buffer Management
    status (*create_command_buffer)(xudk_ctx *ctx, bool is_compute, xudk_gpu_cmd_buffer *cmd_buffer);
    status (*destroy_command_buffer)(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer);
    status (*begin_recording)(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer);
    status (*end_recording)(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer);
    status (*submit_command_buffer)(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer);
    status (*wait_for_completion)(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer);
    
    // Render Pass Management
    status (*begin_render_pass)(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer, xudk_gpu_render_pass *render_pass);
    status (*end_render_pass)(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer);
    status (*set_viewport)(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer, u32 x, u32 y, u32 width, u32 height);
    status (*set_scissor)(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer, u32 x, u32 y, u32 width, u32 height);
    
    // Drawing Commands
    status (*bind_pipeline)(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer, xudk_gpu_pipeline *pipeline);
    status (*bind_vertex_buffers)(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer, u32 first_binding, 
                                 u32 binding_count, xudk_gpu_buffer **buffers, u64 *offsets);
    status (*bind_index_buffer)(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer, xudk_gpu_buffer *buffer, 
                               u64 offset, bool is_32bit);
    status (*draw)(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer, u32 vertex_count, u32 instance_count, 
                  u32 first_vertex, u32 first_instance);
    status (*draw_indexed)(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer, u32 index_count, u32 instance_count,
                          u32 first_index, i32 vertex_offset, u32 first_instance);
    
    // Compute Commands
    status (*dispatch)(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer, u32 group_count_x, u32 group_count_y, u32 group_count_z);
    
    // Resource Binding
    status (*bind_descriptor_set)(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer, u32 set_index, 
                                 handle descriptor_set);
    status (*push_constants)(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer, u32 offset, u32 size, const void *data);
    
    // Synchronization
    status (*insert_barrier)(xudk_ctx *ctx, xudk_gpu_cmd_buffer *cmd_buffer);
    status (*wait_idle)(xudk_ctx *ctx);
    
    // Utility Functions
    status (*present_to_screen)(xudk_ctx *ctx, xudk_gpu_texture *texture);
    status (*capture_screenshot)(xudk_ctx *ctx, const wchar *path);
    status (*get_performance_counters)(xudk_ctx *ctx, u64 *gpu_time, u64 *memory_used, u32 *temperature);
} xudk_gpu;