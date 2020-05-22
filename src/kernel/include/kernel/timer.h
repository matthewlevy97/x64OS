#pragma once

#include <amd64/interrupt/interrupt.h>

#define TIMER_FREQUENCY_HERTZ 1000
#define HERTZ_TO_NANOSECONDS(x) ((x) * 1000)

void timer_init();
void timer_register_interrupt_callback(void (*handler)());

registers_t *timer_interrupt_handler(registers_t *regs);