#pragma once

#include <stdbool.h>
#include <stdint.h>

bool is_atomic();

void atomic_begin();

void atomic_end();
void atomic_end_force();

uint64_t atomic_counter();
void atomic_set_counter(uint64_t counter);