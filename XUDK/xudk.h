#include <stdint.h>
#include <stddef.h>
#include "XUDK/Uefi.h"

// =============================================================================
// CORE TYPES & CONSTANTS
// =============================================================================

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef size_t    usize;
typedef uint16_t  wchar;
typedef uint8_t   bool;
typedef void*     handle;
typedef u64       status;
typedef u64       addr;    // Physical/Virtual address
typedef u32       pid;     // Process/Protocol ID

#define true  1
#define false 0
#define null  ((void*)0)

// Status codes for complex operations
#define XUDK_OK                    0x0000000000000000ULL
#define XUDK_ERROR                 0x8000000000000001ULL
#define XUDK_INVALID_PARAM         0x8000000000000002ULL
#define XUDK_NOT_SUPPORTED         0x8000000000000003ULL
#define XUDK_BUFFER_TOO_SMALL      0x8000000000000005ULL
#define XUDK_NOT_FOUND             0x800000000000000EULL
#define XUDK_OUT_OF_MEMORY         0x8000000000000009ULL
#define XUDK_DEVICE_ERROR          0x8000000000000007ULL
#define XUDK_WRITE_PROTECTED       0x8000000000000008ULL
#define XUDK_ACCESS_DENIED         0x800000000000000CULL
#define XUDK_TIMEOUT               0x8000000000000012ULL
#define XUDK_PROTOCOL_ERROR        0x8000000000000024ULL
#define XUDK_FILESYSTEM_ERROR      0x8000000000000025ULL
#define XUDK_BOOT_FAILURE          0x8000000000000026ULL
#define XUDK_GPU_NOT_FOUND         0x8000000000000027ULL
#define XUDK_GPU_INIT_FAILED       0x8000000000000028ULL
#define XUDK_SHADER_COMPILE_ERROR  0x8000000000000029ULL
#define XUDK_TEXTURE_ERROR         0x800000000000002AULL
#define XUDK_BUFFER_OVERFLOW       0x800000000000002BULL
