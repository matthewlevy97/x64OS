#pragma once

#define KERNEL_OFFSET 0xFFFFFF8000000000

#ifdef __ASSEMBLER__
#define V2P(a) ((a) - KERNEL_OFFSET)
#define P2V(a) ((a) + KERNEL_OFFSET)
#else
#define V2P(a) ((uintptr_t)(a) & ~KERNEL_OFFSET)
#define P2V(a) ((void *)((uintptr_t)(a) | KERNEL_OFFSET))
#endif

#define PAGE_PRESENT       1ULL
#define PAGE_WRITE         (1ULL << 1)
#define PAGE_USER          (1ULL << 2)
#define PAGE_WRITETHROUGH  (1ULL << 3)
#define PAGE_CACHE_DISABLE (1ULL << 4)
#define PAGE_ACCESSED      (1ULL << 5)
#define PAGE_DIRTY         (1ULL << 6)
#define PAGE_HUGE          (1ULL << 7)
#define PAGE_GLOBAL        (1ULL << 8)
//#define PAGE_NX            (1ULL << 63)
#define PAGE_NX            0

#define PAGE_SIZE       0x1000
#define ENTRIES_PER_PT  512

#ifndef __ASSEMBLER__

#include <amd64/amd64.h>
#include <kernel/kernel.h>
#include <stdbool.h>
#include <stdint.h>

#define PAGING_GET_PTR_ADDRESS(pt) ((uintptr_t)(pt) & (~(PAGE_SIZE - 1)))
#define ALIGN_PAGE(x) ALIGN((x), PAGE_SIZE)
#define ALIGN_PAGE_DOWN(x) ALIGN_DOWN((x), PAGE_SIZE)

typedef uint64_t* page_directory_t;
typedef uint64_t vm_flags_t;

page_directory_t vmm_get_page_dir();
page_directory_t vmm_get_boot_page_dir();
void vmm_load_page_dir(page_directory_t page_dir);

page_directory_t vmm_create_page_dir();
page_directory_t vmm_clone_current_page_dir();

void vmm_map_page(uintptr_t physical_address, uintptr_t virtual_address);
void vmm_map_page2(uintptr_t physical_address, uintptr_t virtual_address, vm_flags_t flags);
void vmm_map_page3(page_directory_t page_dir, uintptr_t physical_address, uintptr_t virtual_address, vm_flags_t flags);

void vmm_set_page_flags(uintptr_t virtual_address, vm_flags_t flags);

bool is_virtual_page_present(uintptr_t page);

void dump_memory(page_directory_t p4);

registers_t *page_fault_handler(registers_t *regs);

#endif