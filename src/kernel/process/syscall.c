#include <fs/fs_syscalls.h>
#include <process/scheduler.h>
#include <process/syscall.h>
#include <stddef.h>
#include <string.h>

/*
 * If a syscall is added here, ensure the process/syscall.h::SYSCALL_NUMBER counter increases
*/

uintptr_t syscall_table[] = {
	(uintptr_t)scheduler_run,
	(uintptr_t)do_syscall_open,
	(uintptr_t)do_syscall_close
};

void *copy_user_to_kernel(void *dest, const void *src, size_t nbytes)
{
	// TODO: copy_user_to_kernel(...) is not very safe currently
	return memcpy(dest, src, nbytes);
}

void *copy_kernel_to_user(void *dest, const void *src, size_t nbytes)
{
	// TODO: Implement copy_kernel_to_user(...)
	return 0;
}