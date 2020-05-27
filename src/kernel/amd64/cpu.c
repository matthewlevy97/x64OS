#include <amd64/asm/gdt.h>
#include <amd64/asm/msr.h>
#include <amd64/asm/tss.h>
#include <amd64/common.h>
#include <amd64/cpu.h>
#include <amd64/interrupt/interrupt.h>
#include <amd64/interrupt/interrupt_handlers.h>
#include <amd64/pic.h>
#include <kernel/atomic.h>
#include <kernel/debug.h>
#include <mm/paging.h>

static cpu_t cpu_data = {
	.gdt_ptr  = {
		.size = GDT_SIZE - 1
	},
	.gdt_page = {
		.gdt = {
			[GDT_ENTRY_KERNEL32_CS]         = \
				GDT_ENTRY_INIT((uint64_t)(SEGMENT_PRESENT | SEGMENT_DB | SEGMENT_CODE), 0, 0xFFFFF),
			[GDT_ENTRY_KERNEL_CS]           = \
				GDT_ENTRY_INIT((uint64_t)(SEGMENT_PRESENT | SEGMENT_LONG_MODE | SEGMENT_CODE), 0, 0xFFFFF),
			[GDT_ENTRY_KERNEL_DS]           = \
				GDT_ENTRY_INIT((uint64_t)(SEGMENT_PRESENT | SEGMENT_DB | SEGMENT_DATA), 0, 0xFFFFF),
			[GDT_ENTRY_DEFAULT_USER32_CS]   = \
				GDT_ENTRY_INIT((uint64_t)(SEGMENT_PRESENT | SEGMENT_DB | SEGMENT_CODE | SEGMENT_USER), 0, 0xFFFFF),
			[GDT_ENTRY_DEFAULT_USER_DS]	    = \
				GDT_ENTRY_INIT((uint64_t)(SEGMENT_PRESENT | SEGMENT_DB | SEGMENT_DATA | SEGMENT_USER), 0, 0xFFFFF),
			[GDT_ENTRY_DEFAULT_USER_CS]	    = \
				GDT_ENTRY_INIT((uint64_t)(SEGMENT_PRESENT | SEGMENT_LONG_MODE | SEGMENT_CODE | SEGMENT_USER), 0, 0xFFFFF),
			[GDT_ENTRY_TSS]	    = \
				TSS_ENTRY_INIT()
		}
	}
};

void cpu_init()
{
	atomic_begin();
	
	gdt_init();
	tss_init();
	idt_init();
	pic_init();

	syscall_init();

	// Register some interrupt handlers
	register_interrupt_handler(0x6, invalid_opcode);
	register_interrupt_handler(0xD, general_protection_fault);
	register_interrupt_handler(0xE, page_fault_handler);
	
	// TODO: Want to change the GS.Base to a better structure later on. Any changes will need to be reflected in context_switch.S "gs" offsets

	// Set GS.Base
	wrmsrl(MSR_GSBASE, (uint64_t)&(cpu_data.tss));
	wrmsrl(MSR_KERNEL_GSBASE, (uint64_t)&(cpu_data.tss));

	atomic_end();
}

cpu_t *cpu_get()
{
	return &cpu_data;
}