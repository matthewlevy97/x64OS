#include <drivers/module.h>
#include <fs/initrd/initrd.h>
#include <fs/initrd/ustar.h>
#include <kernel/debug.h>
#include <mm/kmalloc.h>

filesystem_t initrd_fs = {
	.init = initrd_init,
	.destroy = initrd_destroy,
	.open = initrd_open,
	.close = initrd_close,
	.read = initrd_read,
	.write = initrd_write,
	.lseek = initrd_lseek
};

static module_t initrd_module;

int initrd_init()
{
	if(!module_load(&initrd_module, "ramdisk")) {
		return 0;
	}

	return 1;
}

int initrd_destroy()
{
	return 0;
}

file_t initrd_open(const char *filename, struct vfs_file_info *info)
{
	file_t file = (file_t)kmalloc(sizeof(struct __vfs_file));

	if(!file) {
		return NULL;
	}

	if(!ustar_lookup(initrd_module.base_addr, filename, &(file->inode_number))) {
		return NULL;
	}

	file->current_offset = 0;
	return file;
}

void initrd_close(file_t file)
{}

ssize_t initrd_read(file_t file, void *buf, size_t nbyte)
{
	uint64_t file_size;
	
	file_size = ustar_filesize(initrd_module.base_addr, file->inode_number);
	if(file_size > file->current_offset && nbyte > file_size - file->current_offset) {
		nbyte = file_size - file->current_offset;
	}
	
	if(ustar_copy_data(initrd_module.base_addr, file->inode_number, buf, nbyte, file->current_offset)) {
		file->current_offset += nbyte;
		return nbyte;
	}

	return -1;
}

ssize_t initrd_write(file_t file, const void *buf, size_t nbyte)
{
	debug_warning("InitRD is read-only\n");
	return 0;
}

off_t initrd_lseek(file_t file, off_t offset, SeekWhence whence)
{
	file->current_offset = offset;

	return offset;
}