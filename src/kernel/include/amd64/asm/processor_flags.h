#pragma once

// See: https://wiki.osdev.org/EFLAGS#EFLAGS_Register

#define X86_EFLAGS_CF    (1UL << 0)
#define X86_EFLAGS_PF    (1UL << 2)
#define X86_EFLAGS_AF    (1UL << 4)
#define X86_EFLAGS_ZF    (1UL << 6)
#define X86_EFLAGS_SF    (1UL << 7)
#define X86_EFLAGS_TF    (1UL << 8)
#define X86_EFLAGS_IF    (1UL << 9)
#define X86_EFLAGS_DF    (1UL << 10)
#define X86_EFLAGS_OF    (1UL << 11)
#define X86_EFLAGS_IOPL  (3UL << 12)
#define X86_EFLAGS_NT    (1UL << 14)
#define X86_EFLAGS_RF    (1UL << 16)
#define X86_EFLAGS_VM    (1UL << 17)
#define X86_EFLAGS_AC    (1UL << 18)
#define X86_EFLAGS_VIF   (1UL << 19)
#define X86_EFLAGS_VIP   (1UL << 20)
#define X86_EFLAGS_ID    (1UL << 21)

#define X86_EFLAGS_STANDARD (\
	X86_EFLAGS_DF | X86_EFLAGS_IF | \
	X86_EFLAGS_IOPL | X86_EFLAGS_AC)
