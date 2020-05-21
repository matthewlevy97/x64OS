#include <fs/initrd/initrd.h>

filesystem_t initrd_fs = {
	.init = initrd_init,
	.destroy = initrd_destroy,
	.open = initrd_open,
	.read = initrd_read,
	.write = initrd_write,
	.pwrite = initrd_pwrite
};

void initrd_init()
{
	
}

void initrd_destroy()
{

}

file_t initrd_open(const char *filename, struct vfs_file_info *info)
{
	return NULL;
}

void initrd_close(file_t file)
{

}

ssize_t initrd_read(file_t file, void *buf, size_t nbyte)
{
	return 0;
}

ssize_t initrd_write(file_t file, const void *buf, size_t nbyte)
{
	return 0;
}

ssize_t initrd_pwrite(file_t file, const void *buf, size_t nbyte, off_t offset)
{
	return 0;
}