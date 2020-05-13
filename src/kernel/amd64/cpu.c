#include <amd64/interrupt.h>
#include <amd64/interrupt_handlers.h>
#include <amd64/pic.h>
#include <kernel/atomic.h>
#include <kernel/debug.h>
#include <mm/paging.h>

void cpu_init()
{
	atomic_begin();

	idt_init();
	pic_init();

	// Register some interrupt handlers
	register_interrupt_handler(0x6, invalid_opcode);
	register_interrupt_handler(0xD, general_protection_fault);
	register_interrupt_handler(0xE, page_fault_handler);
	
	atomic_end();
}