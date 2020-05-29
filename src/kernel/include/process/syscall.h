#pragma once

#define SYSCALL_YIELD      0
#define SYSCALL_OPEN       1
#define SYSCALL_CLOSE      2
#define SYSCALL_NUMBER     3

#ifndef __ASSEMBLER__

#include <stdint.h>
extern uintptr_t syscall_table[];

void *copy_user_to_kernel(void *dest, const void *src, size_t nbytes);
void *copy_kernel_to_user(void *dest, const void *src, size_t nbytes);

#endif