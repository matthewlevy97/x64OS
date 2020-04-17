#pragma once

#include <amd64/amd64.h>

#define ENABLE_INTERRUPTS()  do { __asm__ volatile ("sti"); } while(0);
#define DISABLE_INTERRUPTS() do { __asm__ volatile ("cli"); } while(0);

#define NUMBER_INTERRUPTS 256
enum {
	IDT_PRESENT = 1 << 7,

	IDT_DPL0    = 0x00, // RING-0

	IDT_INTERRUPT_GATE = 0xE,
	IDT_TRAP_GATE      = 0xF,
};

typedef registers_t *(*int_handler_t)(registers_t *);

void idt_init();

void register_interrupt_handler(uint32_t interrupt_number, int_handler_t handler);
void deregister_interrupt_handler(uint32_t interrupt_number);

registers_t * interrupt_handler(registers_t *regs);