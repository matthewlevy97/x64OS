#pragma once

#include <stddef.h>
#include <sys/types.h>

typedef enum {
	FILE_TYPE_UNKNOWN          = 0x0,
	
	FILE_TYPE_FILE             = 0x1,
	FILE_TYPE_DIRECTORY        = 0x2,
	FILE_TYPE_CHARDEVICE       = 0x3,
	FILE_TYPE_BLOCKDEVICE      = 0x4,
	FILE_TYPE_PIPE             = 0x5,
	FILE_TYPE_SYMLINK          = 0x6,
	
	FILE_TYPE_MOUNTPOINT       = (1 << 3),
} FileType;

struct __vfs_node;
typedef void    (*fs_open_t)(struct __vfs_node *, uint32_t flags);
typedef void    (*fs_close_t)(struct __vfs_node *);

typedef ssize_t (*fs_read_t)(struct __vfs_node *, uint64_t offset, void *buf, size_t nbyte);
typedef ssize_t (*fs_write_t)(struct __vfs_node *, uint64_t offset, const void *buf, size_t nbyte);

typedef struct __vfs_node *(*fs_finddir_t) (struct __vfs_node *, const char *name);