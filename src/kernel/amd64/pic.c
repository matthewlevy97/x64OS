#include <amd64/pic.h>
#include <io/ports.h>
#include <kernel/debug.h>

void pic_init()
{
	// ICW1
	outb(PIC_GET_COMMAND(PIC_MASTER_BASE), PIC_ICW1);
	outb(PIC_GET_COMMAND(PIC_SLAVE_BASE),  PIC_ICW1);

	// ICW2
	outb(PIC_GET_DATA(PIC_MASTER_BASE), IRQ_BASE);
	outb(PIC_GET_DATA(PIC_MASTER_BASE), IRQ_BASE + 8);

	// ICW3
	outb(PIC_GET_DATA(PIC_MASTER_BASE), PIC_ICW3_MASTER);
	outb(PIC_GET_DATA(PIC_MASTER_BASE), PIC_SLAVE_IDENTIFICATION_CODE);

	// ICW4
	outb(PIC_GET_COMMAND(PIC_MASTER_BASE), PIC_ICW4);
	outb(PIC_GET_COMMAND(PIC_SLAVE_BASE),  PIC_ICW4);

	// Register IRQs
	for(uint32_t i = IRQ_BASE; i < IRQ_BASE + IRQ_LENGTH; i++)
		register_interrupt_handler(i, irq_default_handler);

	// Enable only interrupts to slave
	outb(PIC_GET_DATA(PIC_MASTER_BASE), ~PIC_MASTER_TO_SLAVE);
}

uint16_t pic_get_mask()
{
	return ~((inb(PIC_GET_DATA(PIC_SLAVE_BASE)) << 8) | inb(PIC_GET_DATA(PIC_MASTER_BASE)));
}

void pic_write_mask(uint16_t mask)
{
	mask = ~mask;
	outb(PIC_GET_DATA(PIC_MASTER_BASE), mask & 0xFF);
	outb(PIC_GET_DATA(PIC_SLAVE_BASE), (mask >> 8) & 0xFF);
}

void pic_acknowledge(uint32_t interrupt_number)
{
	if(interrupt_number >= IRQ_BASE && interrupt_number - IRQ_BASE >= 8)
		outb(PIC_GET_COMMAND(PIC_SLAVE_BASE), PIC_END_OF_INTERRUPT);

	outb(PIC_GET_COMMAND(PIC_MASTER_BASE), PIC_END_OF_INTERRUPT);
}

registers_t *irq_default_handler(registers_t *regs)
{
	// Acknowledge IRQs
	pic_acknowledge(regs->int_no);

	debug_warning("Unhandled IRQ: 0x%x\n", regs->int_no);

	return regs;
}