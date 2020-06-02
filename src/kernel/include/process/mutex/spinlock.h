#pragma once

#include <stdint.h>

enum {
	SPINLOCK_UNLOCKED = 0,
	SPINLOCK_LOCKED   = 1
};

#define SPINLOCK_INIT SPINLOCK_UNLOCKED

typedef	volatile uint32_t spinlock_t;

void spinlock_acquire(spinlock_t *lock);
void spinlock_release(spinlock_t *lock);

bool spinlock_try_acquire(spinlock_t *lock);