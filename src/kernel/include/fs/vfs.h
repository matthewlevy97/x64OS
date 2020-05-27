#pragma once

#include <sys/types.h>

typedef enum {
	SEEK_SET,
	SEEK_CUR,
	SEEK_END
} SeekWhence;

typedef enum {
	FILE_TYPE_UNKNOWN,
	
	FILE_TYPE_DIRECTORY,
	FILE_TYPE_FILE,
} FileType;

struct vfs_file_info {
	uint32_t flags;
};

struct __vfs_file {
	FileType type;
	uint64_t refcount;
	uint64_t inode_number;
	
	off_t current_offset;
	
	struct vfs_file_info info;
};
typedef struct __vfs_file * file_t;

struct __vfs_filesystem {
	int (*init)();
	int (*destroy)();
	
	file_t (*open)(const char *filename, struct vfs_file_info *info);
	void (*close)(file_t file);
	
	ssize_t (*read)(file_t file, void *buf, size_t nbyte);
	ssize_t (*write)(file_t file, const void *buf, size_t nbyte);
	
	off_t (*lseek)(file_t file, off_t offset, SeekWhence whence);
};
typedef struct __vfs_filesystem filesystem_t;

void vfs_init(filesystem_t *fs);
void vfs_change_rootfs(filesystem_t *fs);

file_t vfs_open(const char *filename, struct vfs_file_info *info);
void vfs_close(file_t file);

ssize_t vfs_read(file_t file, void *buf, size_t nbyte);
ssize_t vfs_write(file_t file, const void *buf, size_t nbuf);

off_t vfs_lseek(file_t file, off_t offset, SeekWhence whence);