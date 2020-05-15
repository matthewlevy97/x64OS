#pragma once

#include <stdint.h>

#define MSR_STAR              0xC0000081 /* Legacy mode SYSCALL */
#define MSR_LSTAR             0xC0000082 /* Long mode SYSCALL */
#define MSR_CSTAR             0xC0000083 /* Compat mode SYSCALL */
#define MSR_SYSCALL_MASK      0xC0000084 /* EFLAGS mask for SYSCALL */

void wrmsr(uint32_t msr, uint32_t low, uint32_t high);
void wrmsrl(uint32_t msr, uint64_t value);

uint64_t rdmsrl(uint32_t msr);