#include <amd64/asm/msr.h>
#include <amd64/asm/segment.h>
#include <amd64/common.h>

void syscall_init()
{
	return;
	// TODO: Setup TSS stuff
	
	// Setup for SYSENTER / SYSEXIT
	wrmsr(MSR_STAR, 0, (__USER32_CS << 16) | __KERNEL_CS);
	wrmsrl(MSR_LSTAR, (unsigned long)0x00); // TODO: Make 0x00 an actual address for SYSENTER to jump to
}