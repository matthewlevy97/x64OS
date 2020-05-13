#include <amd64/timer/pit_8254.h>
#include <kernel/timer.h>

/**
 * Generaic timer wrapper
 */
void timer_init()
{
	pit_init();
}