#include <amd64/interrupt/interrupt.h>
#include <kernel/atomic.h>

/**
 * boot.S disables interrupts immediatly, need to factor that in
 */
static uint64_t atomic_depth = 1;

bool is_atomic()
{
	return (atomic_depth != 0);
}

void atomic_begin()
{
	if(!atomic_depth)
		DISABLE_INTERRUPTS()
	atomic_depth++;
}

void atomic_end()
{
	if(atomic_depth) {
		atomic_depth--;
	}

	if(!atomic_depth) {
		ENABLE_INTERRUPTS();
	}
}

void atomic_end_force()
{
	atomic_depth = 0;
	ENABLE_INTERRUPTS();
}