#include "xudk/uefi.h"

// =============================================================================
//  DATA STRUCTURES
// =============================================================================

//  memory descriptor
typedef struct {
    u32           type;
    u32           pad;
    addr          physical_start;
    addr          virtual_start;
    u64           pages;
    u64           attributes;
    const char*   description;  // Human readable description
} xudk_mem_desc;

// Memory map with management
typedef struct {
    xudk_mem_desc*  descriptors;
    usize           count;
    usize           total_size;
    usize           descriptor_size;
    u32             version;
    u64             total_memory;
    u64             usable_memory;
    u64             reserved_memory;
} xudk_mem_map;

// GPU Information
typedef struct {
    u32                 device_id;
    u32                 vendor_id;
    xudk_gpu_vendor     vendor;
    xudk_gpu_arch       architecture;
    wchar*              device_name;
    wchar*              driver_version;
    u64                 vram_size;
    u64                 shared_memory_size;
    u32                 compute_units;
    u32                 max_texture_size;
    u32                 max_render_targets;
    bool                supports_compute;
    bool                supports_raytracing;
    bool                supports_mesh_shaders;
    bool                supports_variable_rate_shading;
    u32                 max_shader_model;
    addr                mmio_base;
    usize               mmio_size;
} xudk_gpu_info;

// GPU Memory Heap
typedef struct {
    xudk_gpu_mem_type   type;
    u64                 size;
    u64                 available;
    u32                 flags;
    bool                device_local;
    bool                host_visible;
    bool                host_coherent;
} xudk_gpu_heap;

// GPU Buffer
typedef struct {
    handle              buffer_handle;
    u64                 size;
    xudk_buffer_usage   usage;
    void*               mapped_ptr;
    u64                 gpu_address;
    bool                is_mapped;
} xudk_gpu_buffer;

// GPU Texture
typedef struct {
    handle              texture_handle;
    u32                 width;
    u32                 height;
    u32                 depth;
    u32                 mip_levels;
    u32                 array_size;
    xudk_texture_format format;
    u32                 sample_count;
    u64                 size;
    addr                gpu_address;
    bool                is_render_target;
    bool                is_depth_stencil;
} xudk_gpu_texture;

// GPU Shader
typedef struct {
    handle              shader_handle;
    xudk_shader_type    type;
    u32                 bytecode_size;
    void*               bytecode;
    wchar*              entry_point;
    bool                is_compiled;
} xudk_gpu_shader;

// GPU Pipeline State
typedef struct {
    xudk_gpu_shader*           vertex_shader;
    xudk_gpu_shader*           fragment_shader;
    xudk_gpu_shader*           geometry_shader;
    xudk_gpu_shader*           compute_shader;
    xudk_primitive_topology    topology;
    bool                       depth_test_enable;
    bool                       depth_write_enable;
    bool                       blend_enable;
    u32                        render_target_count;
    xudk_texture_format        render_target_formats[8];
    xudk_texture_format        depth_stencil_format;
    u32                        sample_count;
} xudk_gpu_pipeline_desc;

// GPU Pipeline
typedef struct {
    handle                     pipeline_handle;
    xudk_gpu_pipeline_desc     desc;
    bool                       is_compute;
} xudk_gpu_pipeline;

// GPU Command Buffer
typedef struct {
    handle              cmd_buffer_handle;
    bool                is_recording;
    bool                is_compute;
    u32                 level;  // Primary/Secondary
} xudk_gpu_cmd_buffer;

// GPU Render Pass
typedef struct {
    handle              render_pass_handle;
    xudk_gpu_texture**  color_targets;
    xudk_gpu_texture*   depth_target;
    u32                 color_target_count;
    u32                 width;
    u32                 height;
    bool                clear_color;
    bool                clear_depth;
    float               clear_color_value[4];
    float               clear_depth_value;
    u8                  clear_stencil_value;
} xudk_gpu_render_pass;

// Vertex attribute description
typedef struct {
    u32                 location;
    u32                 binding;
    xudk_texture_format format;
    u32                 offset;
} xudk_vertex_attribute;

// Vertex binding description
typedef struct {
    u32                 binding;
    u32                 stride;
    bool                per_instance;
} xudk_vertex_binding;

// File system information
typedef struct {
    wchar*    label;
    wchar*    filesystem_type;
    u64       total_size;
    u64       free_size;
    u64       block_size;
    bool      read_only;
    bool      bootable;
    handle    device_handle;
} xudk_volume_info;

// Disk/Storage information
typedef struct {
    u32       disk_id;
    wchar*    model;
    wchar*    serial;
    u64       total_sectors;
    u32       sector_size;
    bool      removable;
    bool      has_media;
    xudk_volume_info* volumes;
    usize     volume_count;
} xudk_disk_info;

// Network interface information
typedef struct {
    u8        mac_address[6];
    u32       link_speed;
    bool      link_up;
    bool      dhcp_enabled;
    u32       ip_address;
    u32       subnet_mask;
    u32       gateway;
} xudk_net_info;

// System information
typedef struct {
    wchar*    firmware_vendor;
    u32       firmware_revision;
    wchar*    system_vendor;
    wchar*    product_name;
    wchar*    serial_number;
    u64       total_memory;
    u32       cpu_count;
    u32       boot_mode;      // Legacy/UEFI
    bool      secure_boot;
    bool      setup_mode;
} xudk_system_info;

// Boot entry for bootloader management
typedef struct {
    u16       boot_number;
    wchar*    description;
    wchar*    file_path;
    u8*       optional_data;
    usize     optional_data_size;
    bool      active;
    u32       attributes;
} xudk_boot_entry;

// Graphics mode information
typedef struct {
    u32       mode_number;
    u32       horizontal_resolution;
    u32       vertical_resolution;
    u32       pixel_format;
    u32       pixels_per_scanline;
    addr      framebuffer_base;
    usize     framebuffer_size;
} xudk_graphics_mode;

// Enhanced key input with metadata
typedef struct {
    wchar     unicode;
    u16       scan_code;
    u8        shift_state;
    u8        toggle_state;
    u64       timestamp;
} xudk_key_input;

// Time with timezone and DST support
typedef struct {
    u16       year;
    u8        month;
    u8        day;
    u8        hour;
    u8        minute;
    u8        second;
    u32       nanosecond;
    i16       timezone;       // Minutes from UTC
    u8        daylight;       // DST flags
    u64       timestamp;      // Unix timestamp
} xudk_time_info;