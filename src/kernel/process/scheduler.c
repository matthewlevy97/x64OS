#include <amd64/asm/context_switch.h>
#include <amd64/asm/tss.h>
#include <kernel/atomic.h>
#include <kernel/debug.h>
#include <kernel/timer.h>
#include <mm/paging.h>
#include <process/scheduler.h>
#include <utils/linked_list.h>

static List blocked_list;
static List running_list;

static uint64_t time_slice_remaining;

void scheduler_init(process_t process)
{
	uint64_t tmp;

	atomic_begin();

	blocked_list = linked_list_create();
	running_list = linked_list_create();
	scheduler_add_process(process);

	timer_register_interrupt_callback(scheduler_run);

	time_slice_remaining = SCHEDULER_TIME_SLICE_DURATION;
	vmm_load_page_dir(process->page_directory);

	tss_set_rsp(0, (void*)process->stack_pointer);
	tss_set_rsp(1, (void*)process->kernel_stack_pointer);
	
	atomic_set_counter(process->atomic_depth);
	do_context_switch(&tmp, process->stack_pointer, process->page_directory);

	atomic_end();
}

void scheduler_run()
{
	process_t current_process, next_process;

	atomic_begin();

	if(time_slice_remaining) {
		if(time_slice_remaining >= SCHEDULER_TIME_BETWEEN_TICKS)
			time_slice_remaining -= SCHEDULER_TIME_BETWEEN_TICKS;
		else
			time_slice_remaining = 0;

		goto no_task_switch;
	}

	/* Task Switch */
	current_process = linked_list_get_by_index(running_list, 0);
	do {
		linked_list_rotate(running_list);
		next_process = linked_list_get_by_index(running_list, 0);
	} while(next_process->state != PROCESS_RUNNING);

	if(current_process == next_process)
		goto no_task_switch;

	time_slice_remaining = SCHEDULER_TIME_SLICE_DURATION;

	/* Do */
	vmm_load_page_dir(next_process->page_directory);

	tss_set_rsp(0, (void*)next_process->stack_pointer);
	tss_set_rsp(1, (void*)next_process->kernel_stack_pointer);

	current_process->atomic_depth = atomic_counter();

	atomic_set_counter(next_process->atomic_depth);
	
	/* Context Switch */
	do_context_switch(&(current_process->stack_pointer), next_process->stack_pointer, next_process->page_directory);

	/* Undo */
	atomic_set_counter(current_process->atomic_depth);

	tss_set_rsp(0, (void*)current_process->stack_pointer);
	tss_set_rsp(1, (void*)current_process->kernel_stack_pointer);

	vmm_load_page_dir(current_process->page_directory);

	// TODO: Do I want to fall through here???
no_task_switch:
	/* TODO: Clear out blocked list */

	atomic_end();
	return;
}

void scheduler_add_process(process_t process)
{
	atomic_begin();

	linked_list_insert(running_list, process->pid, process);

	atomic_end();
}

process_t get_current_process()
{
	return linked_list_get_by_index(running_list, 0);
}