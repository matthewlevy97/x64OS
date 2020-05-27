#pragma once

/* Extended Feature Enable Register */
#define MSR_EFER              0xC0000080
#define EFER_SCE              1         /* System-Call Extension */
#define EFER_LME              (1 << 8)  /* Long Mode Enable */
#define EFER_NXENABLE         (1 << 11) /* No-eXecutable Enable*/


/* GS.BASE Registers */
#define MSR_GSBASE            0xC0000101
#define MSR_KERNEL_GSBASE     0xC0000102

/* SYSCALL and SYSRET MSRs */
#define MSR_STAR              0xC0000081 /* Legacy mode SYSCALL */
#define MSR_LSTAR             0xC0000082 /* Long mode SYSCALL */
#define MSR_CSTAR             0xC0000083 /* Compat mode SYSCALL */
#define MSR_SYSCALL_MASK      0xC0000084 /* EFLAGS mask for SYSCALL */

#ifndef __ASSEMBLER__

#include <stdint.h>
void wrmsr(uint32_t msr, uint32_t low, uint32_t high);
void wrmsrl(uint32_t msr, uint64_t value);

uint64_t rdmsrl(uint32_t msr);

#endif