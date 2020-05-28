.intel_syntax noprefix

#include <amd64/asm/segment.h>
#include <process/syscall.h>

.extern elf_load

.section .text

/* Thanks to: https://github.com/IsaacWoods/pebble/blob/master/kernel/kernel_x86_64/src/task.s */

/* 
 * NOTE: "gs" offsets are based on GS.Base which is set to &(cpu_data.tss).
 *        Any changes to that value must be reflected here
*/

.global process_entry_trampoline
.global process_entry_trampoline_no_elf_load
process_entry_trampoline:
	call elf_load
process_entry_trampoline_no_elf_load:
	/* Save kernel stack */
	mov gs:0x4, rsp
	mov rsp, gs:0xC
	
	/* Setup for sysret */
	mov rcx, [r15]
	mov r11, r14

	/* Clear registers not cleared on context switch */
	xor r15, r15
	xor r14, r14
	xor rax, rax
	xor rdx, rdx
	xor rsi, rsi
	xor rdi, rdi
	xor r8, r8
	xor r9, r9
	xor r10, r10

	sysretq

.global driver_entry_trampoline
.global driver_entry_trampoline_no_elf_load
driver_entry_trampoline:
	call elf_load
driver_entry_trampoline_no_elf_load:
	/* Setup for iret */
	mov rax, rsp
	pushq __KERNEL_DS
	push rax
	push r14
	pushq __KERNEL_CS
	push [r15]

	/* Clear registers not cleared on context switch */
	xor r15, r15
	xor r14, r14
	xor rax, rax
	xor rcx, rcx
	xor rdx, rdx
	xor rsi, rsi
	xor rdi, rdi
	xor r8, r8
	xor r9, r9
	xor r10, r10
	xor r11, r11

	iretq

.extern syscall_table
.global entry_SYSCALL_64
entry_SYSCALL_64:
	swapgs

	/* Switch to kernel stack */
	mov gs:0xC, rsp
	mov rsp, gs:0x4

	/* Save for SYSRET */
	pushq rcx
	pushq r11

	/* Ensure valid SYSCALL */
	cmp rdi, SYSCALL_NUMBER
	jae .syscall_done

	/* 3rd argument stored in r10 because rcx used by SYSCALL/SYSRET */
	mov rcx, r10

	/* Call correct syscall handler */
	movabs rax, offset syscall_table
	shl rdi, 3
	add rax, rdi
	call [rax]
.syscall_done:

	/* Restore for SYSRET */
	popq r11
	popq rcx

	// TODO: Do I need to clear registers here??

	/* Restore original stack */
	mov gs:0x4, rsp
	mov rsp, gs:0xC

	swapgs

	/* Return to usermode */
	sysretq

.global do_context_switch
do_context_switch:
	push rbx
	push rbp
	push r12
	push r13
	push r14
	push r15

	mov [rdi], rsp
	mov rsp, rsi

	/* Reload page directory */
	mov cr3, rdx

	pop r15
	pop r14
	pop r13
	pop r12
	pop rbp
	pop rbx

	ret