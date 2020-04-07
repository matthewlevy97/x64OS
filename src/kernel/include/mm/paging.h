#pragma once

#define KERNEL_OFFSET 0xFFFFFF8000000000

#ifdef __ASSEMBLER__
#define V2P(a) ((a) - KERNEL_OFFSET)
#define P2V(a) ((a) + KERNEL_OFFSET)
#else

/* C code here */

#endif

#define PAGE_PRESENT      0x001
#define PAGE_WRITE        0x002
#define PAGE_USER         0x004
#define PAGE_HUGE         0x080
#define PAGE_GLOBAL       0x100

#define PAGE_SIZE       0x1000
#define ENTRIES_PER_PT  512