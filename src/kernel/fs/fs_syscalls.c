#include <fs/vfs.h>
#include <mm/kmalloc.h>
#include <process/process.h>
#include <process/scheduler.h>
#include <process/syscall.h>
#include <string.h>

int64_t do_syscall_open(const char *filename, uint32_t open_flags)
{
	char *kernel_fname;
	size_t filename_len;
	int64_t fd;
	process_t current_process;

	if(filename == NULL) {
		return -1;
	}

	filename_len = strlen(filename);
	kernel_fname = kmalloc(filename_len + 1);
	copy_user_to_kernel(kernel_fname, filename, filename_len);
	kernel_fname[filename_len] = '\0';

	current_process = get_current_process();
	fd = process_free_file_descriptor_index(current_process);
	if(fd < 0)
		goto syscall_cleanup;

	current_process->files[fd] = kopen(kernel_fname, open_flags);

syscall_cleanup:
	kfree(kernel_fname);
	return fd;
}

void do_syscall_close(int64_t fd)
{
	process_t current_process;
	
	if(fd >= PROCESS_MAX_FILE_DESCRIPTORS || fd < 0)
		return;

	current_process = get_current_process();
	if(current_process->files && current_process->files[fd]) {
		vfs_close(current_process->files[fd]);
		current_process->files[fd] = NULL;
	}
}