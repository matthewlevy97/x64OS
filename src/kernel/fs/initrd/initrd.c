#include <drivers/module.h>
#include <fs/initrd/initrd.h>
#include <fs/initrd/ustar.h>
#include <kernel/debug.h>
#include <mm/kmalloc.h>
#include <string.h>

static module_t initrd_module;

static vfs_node_t initrd_device_create();

int initrd_init()
{
	if(!module_load(&initrd_module, "ramdisk")) {
		return -1;
	}

	vfs_mount(INITRD_MOUNT_POINT, initrd_device_create());

	return 0;
}

void initrd_open(vfs_node_t node, uint32_t flags)
{
	if(!node) {
		return;
	}

	node->length     = ustar_filesize(initrd_module.base_addr, node->inode);
	node->open_flags = flags;
	node->refcount++;
}

void initrd_close(vfs_node_t node)
{
	node->refcount--;
}

ssize_t initrd_read(vfs_node_t node, uint64_t offset, void *buf, size_t nbyte)
{
	if(!node || !buf)
		return -1;

	if(node->length > offset && nbyte > node->length - offset) {
		nbyte = node->length - offset;
	}
	
	if(ustar_copy_data(initrd_module.base_addr, node->inode, buf, nbyte, offset)) {
		return nbyte;
	}

	return -1;
}

ssize_t initrd_write(vfs_node_t node, uint64_t offset, const void *buf, size_t nbyte)
{
	return nbyte;
}

/*
	Why worry about folders when we can treat this as a flat model
*/
vfs_node_t initrd_findnode(vfs_node_t node, const char *name)
{
	vfs_node_t file;

	if(!node)
		return NULL;
	
	file = vfs_create_node();

	if(name[0] == '/') name++;
	if(!ustar_lookup(initrd_module.base_addr, name, &(file->inode))) {
		vfs_destroy_node(file);
		return NULL;
	}

	file->open  = initrd_open;
	file->close = initrd_close;
	file->read  = initrd_read;
	file->write = initrd_write;

	return file;
}

static vfs_node_t initrd_device_create()
{
	vfs_node_t device;

	device = vfs_create_node();
	strcpy(device->name, INITRD_MOUNT_POINT);
	device->uid     = 0;
	device->gid     = 0;
	device->flags   = FILE_TYPE_CHARDEVICE;
	device->length  = initrd_module.size;
	device->read    = initrd_read;
	device->write   = initrd_write;
	device->open    = initrd_open;
	device->close   = initrd_close;
	device->finddir = initrd_findnode;

	return device;
}