.intel_syntax noprefix

.extern atomic_end_force
.global stack_switch

.section .text
stack_switch:
	push rbp
	mov rbp, rsp

	push r15
	push r14
	push r13
	push r12
	push rbx
	push rbp

	mov [rdi], rsp
	mov rsp, [rsi]

	// Reload CR3 with the new page descriptor
	shl rdx, 25
	shr rdx, 25
	mov cr3, rdx

	// TODO: Should I be starting interrupts here???
	call atomic_end_force

	pop rbp
	pop rbx
	pop r12
	pop r13
	pop r14
	pop r15

	leaveq
	ret