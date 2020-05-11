#pragma once

#define KERNEL_OFFSET 0xFFFFFF8000000000

#ifdef __ASSEMBLER__
#define V2P(a) ((a) - KERNEL_OFFSET)
#define P2V(a) ((a) + KERNEL_OFFSET)
#else
#define V2P(a) ((uintptr_t)(a) & ~KERNEL_OFFSET)
#define P2V(a) ((void *)((uintptr_t)(a) | KERNEL_OFFSET))
#endif

#define PAGE_PRESENT       1
#define PAGE_WRITE         (1 << 1)
#define PAGE_USER          (1 << 2)
#define PAGE_WRITETHROUGH  (1 << 3)
#define PAGE_CACHE_DISABLE (1 << 4)
#define PAGE_ACCESSED      (1 << 5)
#define PAGE_DIRTY         (1 << 6)
#define PAGE_HUGE          (1 << 7)
#define PAGE_GLOBAL        (1 << 8)
#define PAGE_NX            (1ull << 63)

#define PAGE_SIZE       0x1000
#define ENTRIES_PER_PT  512

#ifndef __ASSEMBLER__

#include <amd64/amd64.h>
#include <stdbool.h>
#include <stdint.h>

#define PAGING_GET_PTR_ADDRESS(pt) ((uintptr_t)(pt) & (~(PAGE_SIZE - 1)))

typedef uint64_t* page_directory_t;

page_directory_t vmm_get_page_dir();
void vmm_load_page_dir(page_directory_t page_dir);

page_directory_t vmm_create_page_dir();
page_directory_t vmm_clone_current_page_dir();

void vmm_map_page(uintptr_t physical_address, uintptr_t virtual_address);
void vmm_map_page2(uintptr_t physical_address, uintptr_t virtual_address, uint16_t flags);
void vmm_map_page3(page_directory_t page_dir, uintptr_t physical_address, uintptr_t virtual_address, uint16_t flags);

void vmm_set_page_flags(uintptr_t virtual_address, uint16_t flags);

bool is_virtual_page_present(uintptr_t page);

void dump_memory(page_directory_t p4);

registers_t *page_fault_handler(registers_t *regs);

#endif