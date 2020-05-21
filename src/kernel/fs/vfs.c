#include <fs/vfs.h>
#include <kernel/debug.h>

static filesystem_t *root_fs;

/**
 * XXX: No NULL checks in sight, this isn't good
 */

void vfs_init(filesystem_t *fs)
{
	vfs_change_rootfs(fs);
}

void vfs_change_rootfs(filesystem_t *fs)
{
	if(root_fs) {
		if(!(root_fs->destroy())) {
			PANIC("Failed to destroy current filesystem\n");
		}
	}

	root_fs = fs;
	if(!(root_fs->init())) {
		PANIC("Failed to initialize new filesystem\n");
	}
}

file_t vfs_open(const char *filename, struct vfs_file_info *info)
{
	return root_fs->open(filename, info);
}

void vfs_close(file_t file)
{
	root_fs->close(file);
}

ssize_t vfs_read(file_t file, void *buf, size_t nbyte)
{
	return root_fs->read(file, buf, nbyte);
}

ssize_t vfs_write(file_t file, const void *buf, size_t nbyte)
{
	return root_fs->write(file, buf, nbyte);
}

ssize_t vfs_pwrite(file_t file, const void *buf, size_t nbyte, off_t offset)
{
	return root_fs->pwrite(file, buf, nbyte, offset);
}