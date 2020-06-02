#include <assert.h>
#include <kernel/atomic.h>
#include <process/mutex/spinlock.h>
#include <process/scheduler.h>
#include <stdbool.h>

void spinlock_acquire(spinlock_t *lock)
{
	while(!spinlock_try_acquire(lock)) {
		scheduler_run();
	}
}

void spinlock_release(spinlock_t *lock)
{
	ASSERT(__sync_bool_compare_and_swap(lock, SPINLOCK_LOCKED, SPINLOCK_UNLOCKED));

	atomic_end();
}

bool spinlock_try_acquire(spinlock_t *lock)
{
	atomic_begin();

	if(__sync_bool_compare_and_swap(lock, SPINLOCK_UNLOCKED, SPINLOCK_LOCKED)) {
		return true;
	}

	atomic_end();
	return false;
}