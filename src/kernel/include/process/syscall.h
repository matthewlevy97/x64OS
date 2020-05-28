#pragma once

#define SYSCALL_YIELD      0
#define SYSCALL_NUMBER     1

#ifndef __ASSEMBLER__

#include <stdint.h>
extern uintptr_t syscall_table[];

#endif