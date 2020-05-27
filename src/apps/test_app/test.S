.intel_syntax noprefix

.global _start

.section .text
_start:
	mov rdi, 5
	mov rsi, 3
	syscall
	jmp $