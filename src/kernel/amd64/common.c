#include <amd64/asm/entry64.h>
#include <amd64/asm/msr.h>
#include <amd64/asm/processor_flags.h>
#include <amd64/asm/segment.h>
#include <amd64/common.h>

void syscall_init()
{
	// Setup for SYSENTER / SYSEXIT
	wrmsr(MSR_STAR, 0, (__USER32_CS << 16) | __KERNEL_CS);
	wrmsrl(MSR_LSTAR, (uint64_t)entry_SYSCALL_64);

	wrmsrl(MSR_SYSCALL_MASK, X86_EFLAGS_STANDARD | X86_EFLAGS_NT | X86_EFLAGS_TF);
}

void do_syscall_64(uint64_t nr)
{

}