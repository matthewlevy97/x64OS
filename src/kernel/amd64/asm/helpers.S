.intel_syntax noprefix

#include <amd64/asm/segment.h>

.extern gdt_page

.section .text

.global gdt_reload
gdt_reload:
	lgdt [rdi]

	// Clear out selectors
	xor rax, rax
	mov ss, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	// Reload CS
	popq rax
	pushq __KERNEL_CS
	push rax
	retfq

.global tss_install
tss_install:
	mov ax, __TSS_SELECTOR
	ltr ax
	ret