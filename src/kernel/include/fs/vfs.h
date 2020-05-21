#pragma once

#include <sys/types.h>

typedef enum {
	FILE_TYPE_UNKNOWN,
	
	FILE_TYPE_DIRECTORY,
	FILE_TYPE_FILE,
} FileType;

struct vfs_file_info {
	uint32_t flags;
};

typedef struct {
	uint64_t refcount;
	
	struct vfs_file_info info;
} * file_t;

typedef struct {
	void (*init)();
	void (*destroy)();
	
	file_t (*open)(const char *filename, struct vfs_file_info *info);
	void (*close)(file_t file);
	
	ssize_t (*read)(file_t file, void *buf, size_t nbyte);
	
	ssize_t (*write)(file_t file, const void *buf, size_t nbyte);
	ssize_t (*pwrite)(file_t file, const void *buf, size_t nbyte, off_t offset);
} filesystem_t;

void vfs_init(filesystem_t *fs);
void vfs_change_rootfs(filesystem_t *fs);

file_t vfs_open(const char *filename, struct vfs_file_info *info);
void vfs_close(file_t file);

ssize_t vfs_read(file_t file, void *buf, size_t nbyte);

ssize_t vfs_write(file_t file, const void *buf, size_t nbuf);
ssize_t vfs_pwrite(file_t file, const void *buf, size_t nbuf, off_t offset);