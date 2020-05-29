#pragma once

#include <fs/vfs.h>

#define NULL_DRIVER_MOUNT_POINT "/dev/null"

int null_driver_init();

void null_driver_open(vfs_node_t node, uint32_t flags);
void null_driver_close(vfs_node_t node);

ssize_t null_driver_read(vfs_node_t node, uint64_t offset, void *buf, size_t nbyte);
ssize_t null_driver_write(vfs_node_t node, uint64_t offset, const void *buf, size_t nbyte);

vfs_node_t null_driver_findnode(vfs_node_t node, const char *name);