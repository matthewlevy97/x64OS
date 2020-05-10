#include <process/scheduler.h>
#include <process/stack_switch.h>

static process_t *current_process;

static uint64_t current_process_num;
static uint64_t number_processes;
static process_t *process_list[16];

void scheduler_init(process_t *process)
{
	current_process_num = 0;
	number_processes    = 0;

	process_list[number_processes++] = process;
	current_process = process;

	uint64_t tmp;
	// TODO: This is realllllllly a bad way to do this, but PoC anybody?
	stack_switch(&tmp, &(current_process->thread_list[0]->stack_pointer));
}

void scheduler_run()
{
	// TODO: Make function atomic

	thread_t *current_thread, *next_thread;
	process_t *next_process;

	next_process = process_list[(++current_process_num) % number_processes];
	next_thread = next_process->thread_list[0];
	current_thread = current_process->thread_list[0];

	// TODO: Set TSS
	// TODO: Change page directory if needed

	current_process = next_process;
	stack_switch(&(current_thread->stack_pointer), &(next_thread->stack_pointer));
}

void scheduler_add_process(process_t *process)
{
	process_list[number_processes++] = process;
}

process_t *get_current_process()
{
	return current_process;
}