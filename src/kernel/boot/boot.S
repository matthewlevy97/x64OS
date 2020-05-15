#include <amd64/asm/segment.h>
#include <mm/paging.h>

.intel_syntax noprefix

#define GDT_PRESENT (1 << 15)
#define GDT_CODE    (3 << 11)
#define GDT_LONG    (1 << 21)

// External Functions
.extern kmain
.extern gdt_page

// Exported Functions
.global _start

.section .text

.code32
_start:
	cli
	
	// Save multiboot information
	mov edi, eax
	mov esi, ebx
	
	// Setup stack
	mov esp, offset V2P(boot_stack)
	xor ebp, ebp

	// Enable PAE and PGE
	mov eax, cr4
	or eax, (1 << 5)
	or eax, (1 << 7)
	mov cr4, eax

	// Load a P4 page table
	mov eax, offset V2P(boot_p4)
	mov cr3, eax

	// Enable Long mode
	mov ecx, 0x0C0000080
	rdmsr
	or eax, (1 << 8)
	wrmsr

	// Enable paging
	mov eax, cr0
	or eax, (1 << 31)
	mov cr0, eax

	// Load GDT
	lgdt [V2P(boot_gdt_ptr)]

	// Long jump to long mode
	jmp 0x8:V2P(long_mode_start)

.code64
long_mode_start:
	// Clear out selectors
	xor eax, eax
	mov ss, eax
	mov ds, eax
	mov es, eax

	// Jump into upper memory
	movabs rax, offset upper_memory
	jmp rax

upper_memory:
	// Update stack pointer to upper memory
	mov rax, KERNEL_OFFSET
	add rsp, rax

	// Update multiboot pointer address
	mov rax, KERNEL_OFFSET
	add rsi, rax

	// Remove identity mapping
	xor rax, rax
	movabs [boot_p4], rax

	// Reload page table
	mov rax, cr3
	mov cr3, rax

	// TODO: Get GS_BASE from MSR

	// Reload GDT
	movabs rax, offset boot_gdt_ptr
	lgdt [rax]

	// Clear out selectors
	xor rax, rax
	mov ss, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	// Reload CS
	movabs rax, offset .reload_cs
	pushq 0x8
	push rax
	retfq
.reload_cs:
	// TODO: Restore GS_BASE

	// Jump to C-land
	movabs rax, offset kmain
	call rax

	hlt
	jmp $

.section .rodata
// Single entry GDT
boot_gdt:
	.long 0, 0, 0, (GDT_PRESENT | GDT_CODE | GDT_LONG)
boot_gdt_ptr:
	.short (2 * 8) - 1 // Size of GDT Table - 1
	.quad offset boot_gdt

.section .bss
.align PAGE_SIZE
/*
    Setup stack
*/
.skip PAGE_SIZE
boot_stack: