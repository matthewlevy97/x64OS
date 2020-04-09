#include <amd64/interrupt.h>

void cpu_init()
{
	idt_init();
}