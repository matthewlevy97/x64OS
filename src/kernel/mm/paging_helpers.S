.intel_syntax noprefix

.global vmm_set_cr3
.global vmm_reload_cr3

.section .text
vmm_set_cr3:
	// Shifts are used to clear upper bits. Same as V2P(rdi)
	shl rdi, 25
	shr rdi, 25
	mov cr3, rdi
	ret

vmm_reload_cr3:
	mov rax, cr3
	mov cr3, rax
	ret
