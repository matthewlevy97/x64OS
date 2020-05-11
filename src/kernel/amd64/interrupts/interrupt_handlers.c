#include <amd64/interrupt.h>
#include <amd64/interrupt_handlers.h>
#include <kernel/debug.h>

registers_t *general_protection_fault(registers_t *regs)
{
	PANIC("Protection Fault @ 0x%x\n", regs->rip);
	return regs;
}

registers_t *invalid_opcode(registers_t *regs)
{
	PANIC("Invalid Opcode (0x%x) @ 0x%x\n", *(uint64_t*)(regs->rip), regs->rip);
	return regs;
}