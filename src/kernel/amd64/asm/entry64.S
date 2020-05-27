.intel_syntax noprefix

.extern do_syscall_64

.global entry_SYSCALL_64
.global entry_USERMODE_64

.section .text

entry_SYSCALL_64:
	swapgs

	/* Switch to kernel stack */
	//mov OLD_STACK_POINTER, rsp
	//mov rsp, 0x00000000000

	/* Save RIP */
	pushq rcx

	movq  rdi, rax
	//call  do_syscall_64

	/* Save RIP */
	popq rcx

	/* TODO: Restore original stack */

	/* Return to usermode */
	swapgs
	sysretq

entry_USERMODE_64:
	mov rcx, rdi
	mov rsp, rsi

	/* Probably not needed */
	xor rdi, rdi
	xor rsi, rsi

	sysretq