#pragma once

#include <stdint.h>

int64_t do_syscall_open(const char *filename, uint32_t open_flags);
void do_syscall_close(int64_t fd);