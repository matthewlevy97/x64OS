#include <process/scheduler.h>
#include <process/syscall.h>

/*
 * If a syscall is added here, ensure the process/syscall.h::SYSCALL_NUMBER counter increases
*/

uintptr_t syscall_table[] = {
	(uintptr_t)scheduler_run
};
