#pragma once

#include <fs/types.h>
#include <kernel/kernel.h>
#include <time.h>

typedef struct __vfs_node {
	char            name[256];
	void           *device;
	
	refcount_t      refcount;
	
	mode_t          mode;
	uid_t           uid;
	gid_t           gid;
	
	uint32_t        flags;
	uint32_t        open_flags;
	
	uint64_t        inode;
	uint64_t        length;
	
	time_t          atime; // Access
	time_t          mtime; // Modify
	time_t          ctime; // Create
	
	fs_open_t       open;
	fs_close_t      close;
	fs_read_t       read;
	fs_write_t      write;
	fs_finddir_t    finddir;
} *vfs_node_t;

struct vfs_entry {
	char       *name;
	vfs_node_t  file;
	char       *device;
	char       *fs_type;
};

void vfs_init();
void vfs_mount(const char *path, vfs_node_t local_root);

vfs_node_t vfs_create_node();
void vfs_destroy_node(vfs_node_t node);

/* FileSystem Operations */
void        vfs_open(vfs_node_t node, uint32_t flags);
void        vfs_close(vfs_node_t node);
ssize_t     vfs_read(vfs_node_t node, uint64_t offset, void *buf, size_t nbyte);
ssize_t     vfs_write(vfs_node_t node, uint64_t offset, const void *buf, size_t nbyte);
vfs_node_t  vfs_finddir(vfs_node_t node, const char *name);

vfs_node_t  kopen(const char *filename, uint32_t flags);

#ifdef VFS_DEBUG_MOUNTS
void vfs_display_mount_tree();
#endif