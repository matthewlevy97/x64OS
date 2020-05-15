#pragma once

#include <amd64/interrupt/interrupt.h>

void timer_init();
void timer_register_interrupt_callback(void (*handler)());

registers_t *timer_interrupt_handler(registers_t *regs);