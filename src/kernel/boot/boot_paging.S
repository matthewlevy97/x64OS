.intel_syntax noprefix

#include <mm/paging.h>

.global boot_p4

.section .data
.align PAGE_SIZE

/*
    Setup 4-tier paging and memory map first 2MB
*/
boot_p4:
    .quad offset V2P(boot_p3) + (PAGE_PRESENT | PAGE_WRITE) // Identity mapping
    .rept ENTRIES_PER_PT - 2
        .quad 0
    .endr
    .quad offset V2P(boot_p3) + (PAGE_PRESENT | PAGE_WRITE) // Higher-half mapping
boot_p3:
    .quad offset V2P(boot_p2) + (PAGE_PRESENT | PAGE_WRITE)
    .rept ENTRIES_PER_PT - 1
        .quad 0
    .endr
boot_p2:
    .quad offset V2P(boot_p1) + (PAGE_PRESENT | PAGE_WRITE)
    .rept ENTRIES_PER_PT - 1
        .quad 0
    .endr
boot_p1:
    .set i, 0
    .rept ENTRIES_PER_PT
        .quad (i << 12) + (PAGE_PRESENT | PAGE_WRITE)
        .set i, (i+1)
    .endr