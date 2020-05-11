#pragma once

#define ALIGN(x, y) (((x) + ((y)-1)) & ~((y)-1))

#define ALIGN_8(x) ALIGN(x, 8)
#define ALIGN_16(x) ALIGN(x, 16)
#define ALIGN_32(x) ALIGN(x, 32)
#define ALIGN_64(x) ALIGN(x, 64)

#define incptr(ptr, x) ((void*)(((char*)(ptr)) + (x)))
#define decptr(ptr, x) ((void*)(((char*)(ptr)) - (x)))

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))