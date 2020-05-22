#include <kernel/atomic.h>
#include <kernel/debug.h>
#include <kernel/kernel.h>
#include <mm/kmalloc.h>
#include <mm/maddrs.h>
#include <mm/paging_helpers.h>
#include <mm/pmm.h>
#include <process/process.h>
#include <process/stack_switch.h>
#include <string.h>

static uint64_t current_pid = 0;

static void setup_stack(process_t *proc);

process_t *process_create(process_t *parent_proc, const char *process_name, bool kernel_mode, void (*entry_point)(void))
{
	process_t *proc;
	
	atomic_begin();

	proc = kmalloc(sizeof(process_t));
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

	strncpy(proc->name, process_name, sizeof(proc->name));
	proc->name[sizeof(proc->name) - 1] = '\0'; // Ensure NULL terminated

	proc->kernel_mode = false;
	if(kernel_mode) {
		if(!parent_proc || (parent_proc && parent_proc->kernel_mode)) {
			proc->kernel_mode = true;
		}
	}

	proc->state = PROCESS_IDLE;
	proc->exit_value = 0;

	proc->entry_point = entry_point;

	proc->tid      = 1;
	proc->next_tid = proc->tid + 1;

	setup_stack(proc);

	atomic_end();

	return proc;
}

static void setup_stack(process_t *proc)
{
	//page_directory_t current_page_dir, current_local_proc_storage;
	struct stack_switch_registers *regs;
	void *stack_base;
	
	proc->page_directory = vmm_create_page_dir();

	proc->stack_pointer = (void*)USER_SPACE_STACK_START;
	proc->tss_stack_pointer = (void*)KERNEL_STACK_START;

	// Create stack used on interrupt calls
	stack_base = (void*)pmm_calloc();
	vmm_map_page3(proc->page_directory,
		(uintptr_t)stack_base,
		(uintptr_t)decptr(proc->tss_stack_pointer, PAGE_SIZE),
		PAGE_WRITE | PAGE_PRESENT);

	// Create main stack if needed
	stack_base = (void*)pmm_calloc();
	vmm_map_page3(proc->page_directory,
		(uintptr_t)stack_base,
		(uintptr_t)decptr(proc->stack_pointer, PAGE_SIZE),
		PAGE_WRITE | PAGE_PRESENT | (!proc->kernel_mode ? PAGE_USER : 0));

	// Setup stack values
	regs      = incptr(P2V(stack_base), PAGE_SIZE - sizeof(struct stack_switch_registers));
	regs->rbp = (uint64_t)decptr(proc->stack_pointer, sizeof(uint64_t) * 2);
	regs->ret = (uint64_t)proc->entry_point;

	proc->stack_pointer = decptr(proc->stack_pointer, sizeof(struct stack_switch_registers));
}

void dump_process(process_t *process)
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
	debug("TSS Stack:   0x%x\n", process->tss_stack_pointer);
	debug("TID:         %d\n", process->tid);

	debug("Memory Map:\n");
	dump_memory(process->page_directory);
	debug("---------------------\n");
}
