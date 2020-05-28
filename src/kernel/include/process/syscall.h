#pragma once

#define NUMBER_SYSCALLS 1

#ifndef __ASSEMBLER__

#include <stdint.h>
extern uintptr_t syscall_table[];

#endif