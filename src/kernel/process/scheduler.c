#include <kernel/atomic.h>
#include <kernel/debug.h>
#include <kernel/timer.h>
#include <mm/paging.h>
#include <mm/paging_helpers.h>
#include <process/scheduler.h>
#include <process/stack_switch.h>
#include <utils/linked_list.h>

static List process_list;

void scheduler_init(process_t *process)
{
	uint64_t tmp;

	atomic_begin();

	process_list = linked_list_create();
	scheduler_add_process(process);

	timer_register_interrupt_callback(scheduler_run);
	
	vmm_load_page_dir(process->page_directory);

	stack_switch(&tmp, &(process->stack_pointer), process->page_directory);
}

void scheduler_run()
{
	process_t *prev_process, *current_process;
	
	atomic_begin();
	
	prev_process = get_current_process();

	linked_list_rotate(process_list);
	current_process = get_current_process();

	if(prev_process == current_process) return;

	// TODO: Set TSS
	
	vmm_load_page_dir(current_process->page_directory);

	// Note: Atomic end ("sti") implicitly called in stack_switch
	// Switch stacks
	stack_switch(&(prev_process->stack_pointer), &(current_process->stack_pointer), current_process->page_directory);
}

void scheduler_add_process(process_t *process)
{
	linked_list_insert(process_list, process->pid, process);
}

process_t *get_current_process()
{
	return (process_t*)linked_list_get_by_index(process_list, 0);
}