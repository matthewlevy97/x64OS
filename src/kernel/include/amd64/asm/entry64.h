#pragma once

#include <stdint.h>

void entry_SYSCALL_64();
void entry_USERMODE_64(uintptr_t rip, uintptr_t rsp);