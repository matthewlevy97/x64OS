#include <mm/paging.h>
#include <mm/pmm.h>
#include <process/stack_switch.h>
#include <process/thread.h>

thread_t *thread_create(uint64_t tid, void (*entry_point)(void))
{
	// TODO: Make function atomic

	thread_t *thread;
	struct stack_switch_registers *regs;

	thread                = P2V(pmm_calloc());
	thread->tid           = tid;

	// TODO: Make the thread stack larger than 1 page
	thread->stack_pointer = P2V(pmm_calloc());
	thread->state         = THREAD_IDLE;

	regs      = thread->stack_pointer;
	regs->rbp = (uint64_t)&regs->rbp2;
	regs->ret = (uint64_t)entry_point;

	return thread;
}
