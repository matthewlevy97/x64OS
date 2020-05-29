#pragma once

#include <fs/vfs.h>

#define INITRD_MOUNT_POINT "/dev/initrd"

int initrd_init();

void initrd_open(vfs_node_t node, uint32_t flags);
void initrd_close(vfs_node_t node);

ssize_t initrd_read(vfs_node_t node, uint64_t offset, void *buf, size_t nbyte);
ssize_t initrd_write(vfs_node_t node, uint64_t offset, const void *buf, size_t nbyte);

vfs_node_t initrd_findnode(vfs_node_t node, const char *name);