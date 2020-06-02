#pragma once

#include <stdint.h>

enum {
	SPIN_UNLOCKED = 0,
	SPIN_LOCKED   = 1
};

typedef	volatile uint32_t spinlock_t;

void spinlock_acquire(spinlock_t *lock);
void spinlock_release(spinlock_t *lock);

bool spinlock_try_acquire(spinlock_t *lock);