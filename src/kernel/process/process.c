#include <kernel/debug.h>
#include <mm/pmm.h>
#include <process/process.h>
#include <string.h>

static uint64_t current_pid = 0;

process_t *process_create(const char *process_name, bool user, page_directory_t page_directory, void (*entry_point)(void))
{
	// TODO: Make function atomic

	process_t *proc;

	proc = P2V(pmm_calloc());
	proc->pid = ++current_pid;

	strncpy(proc->name, process_name, sizeof(proc->name));
	proc->name[sizeof(proc->name) - 1] = '\0'; // Ensure NULL terminated

	proc->user = user;

	proc->state = PROCESS_IDLE;
	proc->exit_value = 0;
	proc->page_directory = page_directory;

	// TODO: Make the kernel stack larger than 1 page
	// TODO: Set TSS to this value on a task-switch
	proc->kernel_stack = P2V(pmm_calloc());

	proc->entry_point = entry_point;

	proc->tid_counter = 0;

	// TODO: Initialize: proc->thread_list to something dynamic
	proc->thread_list[0] = thread_create(++(proc->tid_counter), entry_point);

	return proc;
}

void dump_process(process_t *process)
{
	debug("---------------------\n");
	debug("PID: %d\n", process->pid);
	debug("Name: %s\n", process->name);
	debug("User: %d\n", process->user);
	debug("State: %d\n", process->state);
	debug("Entry Point: 0x%x\n", process->entry_point);
	debug("Kernel Stack: 0x%x\n", process->kernel_stack);
	debug("TID Counter: %d\n", process->tid_counter);
	debug("---------------------\n");
}