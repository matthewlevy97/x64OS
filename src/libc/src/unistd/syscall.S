.intel_syntax noprefix

.global syscall

.section .text

/*
	Syscall Number: rdi
	Arguments:      rsi, rdx, r10, r8, r9
*/
syscall:
	// Get number of arguments
	mov rax, rsi

	mov rsi, rdx
	mov rdx, r10
	mov r10, r8
	mov r8,  r9

	cmp rax, 5
	ja 2f
	jb 1f

	// Load 5th argument
	popq r9
1:
	// TODO: Should I go through and clear out unused registers here?
	syscall
	jmp 3f
2:
	// Too many arguments
	// TODO: Something else besides returning -1 should be done here, I think
	mov rax, -1
3:
	ret