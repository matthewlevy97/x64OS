#include <amd64/interrupt.h>
#include <amd64/pic.h>

void cpu_init()
{
	DISABLE_INTERRUPTS();

	idt_init();

	pic_init();

	ENABLE_INTERRUPTS();
}