#pragma once

#include <fs/vfs.h>

extern filesystem_t initrd_fs;

int initrd_init();
int initrd_destroy();

file_t initrd_open(const char *filename, struct vfs_file_info *info);
void initrd_close(file_t file);

ssize_t initrd_read(file_t file, void *buf, size_t nbyte);

ssize_t initrd_write(file_t file, const void *buf, size_t nbyte);
ssize_t initrd_pwrite(file_t file, const void *buf, size_t nbyte, off_t offset);