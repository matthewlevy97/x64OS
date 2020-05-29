#pragma once

#include <stdint.h>

typedef uint64_t off_t;

typedef uint32_t mode_t;
typedef uint32_t uid_t;
typedef uint32_t gid_t;

// For Xseek()
typedef enum {
	SEEK_SET,
	SEEK_CUR,
	SEEK_END
} SeekWhence;