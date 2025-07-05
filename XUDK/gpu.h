// =============================================================================
// GPU TYPES & ENUMERATIONS
// =============================================================================

// GPU vendor identification
typedef enum {
    XUDK_GPU_VENDOR_UNKNOWN = 0,
    XUDK_GPU_VENDOR_NVIDIA,
    XUDK_GPU_VENDOR_AMD,
    XUDK_GPU_VENDOR_INTEL,
    XUDK_GPU_VENDOR_ARM,
    XUDK_GPU_VENDOR_QUALCOMM,
    XUDK_GPU_VENDOR_IMAGINATION
} xudk_gpu_vendor;

// GPU architecture types
typedef enum {
    XUDK_GPU_ARCH_UNKNOWN = 0,
    XUDK_GPU_ARCH_LEGACY,
    XUDK_GPU_ARCH_UNIFIED,
    XUDK_GPU_ARCH_COMPUTE,
    XUDK_GPU_ARCH_RAYTRACING
} xudk_gpu_arch;

// GPU memory types
typedef enum {
    XUDK_GPU_MEM_VRAM = 0,
    XUDK_GPU_MEM_SYSTEM,
    XUDK_GPU_MEM_SHARED,
    XUDK_GPU_MEM_UNIFIED
} xudk_gpu_mem_type;

// Shader types
typedef enum {
    XUDK_SHADER_VERTEX = 0,
    XUDK_SHADER_FRAGMENT,
    XUDK_SHADER_GEOMETRY,
    XUDK_SHADER_COMPUTE,
    XUDK_SHADER_TESSELLATION_CONTROL,
    XUDK_SHADER_TESSELLATION_EVALUATION
} xudk_shader_type;

// Texture formats
typedef enum {
    XUDK_FORMAT_UNKNOWN = 0,
    XUDK_FORMAT_R8G8B8A8_UNORM,
    XUDK_FORMAT_R8G8B8A8_SRGB,
    XUDK_FORMAT_B8G8R8A8_UNORM,
    XUDK_FORMAT_B8G8R8A8_SRGB,
    XUDK_FORMAT_R32G32B32A32_FLOAT,
    XUDK_FORMAT_R16G16B16A16_FLOAT,
    XUDK_FORMAT_R32G32_FLOAT,
    XUDK_FORMAT_R32_FLOAT,
    XUDK_FORMAT_D32_FLOAT,
    XUDK_FORMAT_D24_UNORM_S8_UINT,
    XUDK_FORMAT_BC1_UNORM,
    XUDK_FORMAT_BC2_UNORM,
    XUDK_FORMAT_BC3_UNORM,
    XUDK_FORMAT_BC4_UNORM,
    XUDK_FORMAT_BC5_UNORM,
    XUDK_FORMAT_BC6H_UF16,
    XUDK_FORMAT_BC7_UNORM
} xudk_texture_format;

// Primitive topology
typedef enum {
    XUDK_TOPOLOGY_POINTS = 0,
    XUDK_TOPOLOGY_LINES,
    XUDK_TOPOLOGY_LINE_STRIP,
    XUDK_TOPOLOGY_TRIANGLES,
    XUDK_TOPOLOGY_TRIANGLE_STRIP,
    XUDK_TOPOLOGY_TRIANGLE_FAN
} xudk_primitive_topology;

// Buffer usage flags
typedef enum {
    XUDK_BUFFER_VERTEX    = 0x01,
    XUDK_BUFFER_INDEX     = 0x02,
    XUDK_BUFFER_UNIFORM   = 0x04,
    XUDK_BUFFER_STORAGE   = 0x08,
    XUDK_BUFFER_DYNAMIC   = 0x10,
    XUDK_BUFFER_STAGING   = 0x20
} xudk_buffer_usage;