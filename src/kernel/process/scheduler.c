#include <kernel/atomic.h>
#include <kernel/debug.h>
#include <kernel/timer.h>
#include <mm/paging.h>
#include <mm/paging_helpers.h>
#include <process/scheduler.h>
#include <process/stack_switch.h>
#include <utils/linked_list.h>

static List blocked_list;
static List running_list;

static process_t *active_process;

static uint64_t time_slice_remaining;

void scheduler_init(process_t *process)
{
	uint64_t tmp;

	atomic_begin();

	blocked_list = linked_list_create();
	running_list = linked_list_create();
	scheduler_add_process(process);
	set_active_process(process);

	timer_register_interrupt_callback(scheduler_run);
	
	vmm_load_page_dir(process->page_directory);

	time_slice_remaining = 0;
	stack_switch(&tmp, &(process->stack_pointer), process->page_directory);
}

void scheduler_run()
{
	process_t *prev_process, *current_process;
	
	atomic_begin();
	
	prev_process = get_current_process();

	linked_list_rotate(running_list);
	current_process = get_current_process();

	while(current_process->state != PROCESS_RUNNING && current_process != prev_process) {
		// Move process to blocked list
		linked_list_remove_by_index(running_list, 0);
		linked_list_insert(blocked_list, current_process->pid, current_process);

		linked_list_rotate(running_list);
		current_process = get_current_process();
	}
	if(current_process == prev_process)
		goto no_task_switch;

	// TODO: Set TSS
	
	if(!time_slice_remaining) {
		time_slice_remaining = SCHEDULER_TIME_SLICE_DURATION;

		vmm_load_page_dir(current_process->page_directory);

		// Note: Atomic end called in stack_switch
		stack_switch(&(prev_process->stack_pointer), &(current_process->stack_pointer), current_process->page_directory);
	} else {
		goto no_task_switch;
	}

	return;
no_task_switch:
	if(time_slice_remaining >= SCHEDULER_TIME_BETWEEN_TICKS)
		time_slice_remaining -= SCHEDULER_TIME_BETWEEN_TICKS;
	else
		time_slice_remaining = 0;

	// Unblock processes
	// TODO: I believe that rotate -> get_index(0) is faster than get_index(i)
	for(int i = linked_list_length(blocked_list); i--;) {
		linked_list_rotate(running_list);
		current_process = linked_list_get_by_index(blocked_list, 0);

		if(current_process->state == PROCESS_RUNNING) {
			linked_list_remove_by_index(blocked_list, 0);
			linked_list_insert(running_list, current_process->pid, current_process);
		}
	}

	atomic_end();
	return;
}

void scheduler_add_process(process_t *process)
{
	linked_list_insert(running_list, process->pid, process);
}

process_t *get_current_process()
{
	return (process_t*)linked_list_get_by_index(running_list, 0);
}

process_t *get_active_process()
{
	return active_process;
}
void set_active_process(process_t *process)
{
	active_process = process;
}