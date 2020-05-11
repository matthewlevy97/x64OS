#include <mm/paging.h>
#include <mm/paging_helpers.h>
#include <process/scheduler.h>
#include <process/stack_switch.h>
#include <kernel/debug.h>

static process_t *current_process;

static uint64_t current_process_num;
static uint64_t number_processes;
static process_t *process_list[16];

void scheduler_init(process_t *process)
{
	uint64_t tmp;

	current_process_num = 0;
	number_processes    = 0;

	scheduler_add_process(process);
	current_process = process;
	
	vmm_load_page_dir(current_process->page_directory);

	stack_switch(&tmp, &(current_process->stack_pointer), current_process->page_directory);
}

void scheduler_run()
{
	process_t *prev_process;
	// TODO: Make function atomic
	
	prev_process = current_process;
	current_process = process_list[++current_process_num % number_processes];
	if(prev_process == current_process) return;

	// TODO: Set TSS
	
	vmm_load_page_dir(current_process->page_directory);

	// Switch stacks
	stack_switch(&(prev_process->stack_pointer), &(current_process->stack_pointer), current_process->page_directory);
}

void scheduler_add_process(process_t *process)
{
	process_list[number_processes++] = process;
}

process_t *get_current_process()
{
	return current_process;
}