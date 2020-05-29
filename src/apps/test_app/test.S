.intel_syntax noprefix

.global _start

.section .text
_start:
	mov rdi, 1
	lea rsi, filename
	mov rdx, 0
	syscall

	jmp $

.section .data
filename:
	.string "/dev/null\0"