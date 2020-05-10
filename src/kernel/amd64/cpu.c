#include <amd64/interrupt.h>
#include <amd64/pic.h>
#include <kernel/debug.h>

void cpu_init()
{
	DISABLE_INTERRUPTS();

	idt_init();

	pic_init();

	debug_info("CPU Initialized\n");
	ENABLE_INTERRUPTS();
}