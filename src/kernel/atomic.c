#include <amd64/interrupt/interrupt.h>
#include <kernel/atomic.h>

// TODO: Might want to move the atomic_depth counter inside the process_t structure

static volatile uint64_t atomic_depth = 0;

bool is_atomic()
{
	return (atomic_depth != 0);
}

void atomic_begin()
{
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

uint64_t atomic_counter()
{
	return atomic_depth;
}
void atomic_set_counter(uint64_t counter)
{
	atomic_depth = counter;
}