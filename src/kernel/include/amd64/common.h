#pragma once

#include <stdint.h>

void syscall_init();
void do_syscall_64(uint64_t nr);