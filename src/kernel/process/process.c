#include <amd64/asm/context_switch.h>
#include <amd64/asm/processor_flags.h>
#include <drivers/elf.h>
#include <kernel/atomic.h>
#include <kernel/debug.h>
#include <kernel/kernel.h>
#include <mm/kmalloc.h>
#include <mm/maddrs.h>
#include <mm/pmm.h>
#include <process/process.h>
#include <string.h>

static uint64_t current_pid = 0;

static void setup_stack(process_t proc);

process_t process_create(process_t parent_proc, const char *path, bool kernel_mode)
{
	process_t proc;
	
	atomic_begin();

	proc = kmalloc(sizeof(struct __process));
	proc->pid = ++current_pid;
	
	if(parent_proc) {
		proc->ppid = parent_proc->pid;

		proc->sibling            = parent_proc->first_child;
		parent_proc->first_child = proc;
	} else {
		proc->ppid = 0;

		proc->first_child = NULL;
		proc->sibling     = NULL;
	}

	strncpy(proc->name, path, sizeof(proc->name));
	proc->name[sizeof(proc->name) - 1] = '\0'; // Ensure NULL terminated

	proc->kernel_mode = false;
	if(kernel_mode) {
		if(!parent_proc || (parent_proc && parent_proc->kernel_mode)) {
			proc->kernel_mode = true;
		}
	}

	proc->atomic_depth = 0;

	proc->state = PROCESS_RUNNING;
	proc->exit_value = 0;

	proc->entry_point = 0;

	proc->tid      = 1;
	proc->next_tid = proc->tid + 1;

	setup_stack(proc);

	atomic_end();

	return proc;
}

void dump_process(process_t process)
{
	debug("---------------------\n");
	debug("PID:         %d\n", process->pid);
	debug("PPID:        %d\n", process->ppid);
	debug("Name:        \"%s\"\n", process->name);
	debug("Kernel Mode: %d\n", process->kernel_mode);
	debug("State:       %d\n", process->state);
	debug("Exit Value:  %d\n", process->exit_value);
	debug("Entry Point: 0x%x\n", process->entry_point);
	debug("Stack:       0x%x\n", process->stack_pointer);
	debug("TSS Stack:   0x%x\n", process->kernel_stack_pointer);
	debug("TID:         %d\n", process->tid);
	debug("Page Dir:    0x%x\n", process->page_directory);

	debug("Memory Map:\n");
	dump_memory(process->page_directory);
	debug("---------------------\n");
}

/* TODO: Setup call stack for flow:
 * USERMODE:   do_context_switch() -> process_entry_trampoline() -> process
 * KERNELMODE: do_context_switch() -> driver_entry_trampoline() -> process
 */
static inline void setup_stack(process_t proc)
{
	struct stack_switch_registers *regs;
	void *stack_base, *kernel_stack_base;
	
	proc->page_directory       = vmm_create_page_dir();
	proc->kernel_stack_pointer = (uintptr_t)KERNEL_STACK_START;
	proc->stack_pointer        = (uintptr_t)USER_SPACE_STACK_START;

	// Create stack used by kernel
	kernel_stack_base = (void*)pmm_calloc();
	vmm_map_page3(proc->page_directory,
		(uintptr_t)kernel_stack_base,
		(uintptr_t)decptr(proc->kernel_stack_pointer, PAGE_SIZE),
		PAGE_NX | PAGE_WRITE | PAGE_PRESENT);

	// Create stack user by program
	stack_base = (void*)pmm_calloc();
	vmm_map_page3(proc->page_directory,
		(uintptr_t)stack_base,
		(uintptr_t)decptr(proc->stack_pointer, PAGE_SIZE),
		PAGE_NX | PAGE_WRITE | PAGE_PRESENT | (!proc->kernel_mode ? PAGE_USER : 0));

	// Setup stack values
	regs       = incptr(P2V(kernel_stack_base), PAGE_SIZE - sizeof(struct stack_switch_registers));
	regs->r15  = (uint64_t)&(proc->entry_point);
	regs->r14  = X86_EFLAGS_STANDARD;
	regs->r13  = 0;
	regs->r12  = 0;
	regs->rbp  = 0;
	regs->ret  = (uintptr_t)(proc->kernel_mode ? driver_entry_trampoline : process_entry_trampoline);
	proc->kernel_stack_pointer = (uint64_t)decptr(proc->kernel_stack_pointer, sizeof(struct stack_switch_registers));

	/**
	* Setup everything on kernel stack
	* If kernel process -> stack_pointer == kernel_stack
	* Else: kernel_stack = user_stack_pointer; stack_pointer = kernel_stack
	*/
	proc->stack_pointer = proc->kernel_stack_pointer;
	if(!proc->kernel_mode) {
		proc->kernel_stack_pointer = (uintptr_t)USER_SPACE_STACK_START;
	}
}