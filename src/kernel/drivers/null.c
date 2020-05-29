#include <drivers/module.h>
#include <drivers/null.h>
#include <string.h>

static vfs_node_t device_create();

int null_driver_init()
{
	vfs_mount(NULL_DRIVER_MOUNT_POINT, device_create());

	return 0;
}

void null_driver_open(vfs_node_t node, uint32_t flags)
{}

void null_driver_close(vfs_node_t node)
{}

ssize_t null_driver_read(vfs_node_t node, uint64_t offset, void *buf, size_t nbyte)
{
	if(!node)
		return -1;

	if(!buf)
		return -1;

	memset(buf, 0, nbyte);

	return nbyte;
}

ssize_t null_driver_write(vfs_node_t node, uint64_t offset, const void *buf, size_t nbyte)
{
	return nbyte;
}

vfs_node_t null_driver_findnode(vfs_node_t node, const char *name)
{
	vfs_node_t file;

	if(!node)
		return NULL;
	
	file = vfs_create_node();
	file->inode = 0;
	file->open  = null_driver_open;
	file->close = null_driver_close;
	file->read  = null_driver_read;
	file->write = null_driver_write;

	return file;
}

static vfs_node_t device_create()
{
	vfs_node_t device;

	device = vfs_create_node();
	strcpy(device->name, NULL_DRIVER_MOUNT_POINT);
	device->uid     = 0;
	device->gid     = 0;
	device->flags   = FILE_TYPE_BLOCKDEVICE;
	device->length  = 0;
	device->read    = null_driver_read;
	device->write   = null_driver_write;
	device->open    = null_driver_open;
	device->close   = null_driver_close;
	device->finddir = null_driver_findnode;

	return device;
}