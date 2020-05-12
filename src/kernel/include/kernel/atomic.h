#pragma once

#include <stdbool.h>

bool is_atomic();

void atomic_begin();

void atomic_end();
void atomic_end_force();