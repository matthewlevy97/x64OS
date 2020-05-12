#include <amd64/pic.h>
#include <amd64/timer/pit_8254.h>
#include <io/ports.h>
#include <kernel/debug.h>

void pit_init()
{
	// TODO: Remove magic numbers for PIT initialization
	outb(PIT_8254_CMD, 0x68);
	outb(PIT_8254_DATA_CHANNEL0, 0xa9);
	outb(PIT_8254_DATA_CHANNEL0, 0x04);

	// Note: PIT IRQs enabled by the scheduler
}
