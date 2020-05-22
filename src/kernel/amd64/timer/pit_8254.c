#include <amd64/pic.h>
#include <amd64/timer/pit_8254.h>
#include <io/ports.h>
#include <kernel/debug.h>
#include <kernel/timer.h>

void pit_init()
{
	uint16_t divisor;

	debug_info("PIT Initialized with frequency %dHz\n", TIMER_FREQUENCY_HERTZ);
	divisor = (uint16_t)(1193181 / (uint16_t)TIMER_FREQUENCY_HERTZ);

	outb(PIT_8254_CMD, 0x68);
	outb(PIT_8254_DATA_CHANNEL0, divisor & 0xFF);
	outb(PIT_8254_DATA_CHANNEL0, (divisor >> 8) & 0xFF);

	// Note: PIT IRQs enabled by the scheduler
}
