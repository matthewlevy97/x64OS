#pragma once

/*
 * Information Sheets:
 * 	https://www.intel.com/content/dam/www/public/us/en/documents/datasheets/7-series-chipset-pch-datasheet.pdf
 */

#include <amd64/interrupt/interrupt.h>
#include <stdint.h>

#define PIC_GET_COMMAND(x)             (x)
#define PIC_GET_DATA(x)                ((x) + 1)

#define PIC_END_OF_INTERRUPT           0x20

#define PIC_MASTER_BASE                0x20
#define PIC_SLAVE_BASE                 0xA0

#define PIC_ICW1                       (1 << 4)

#define PIC_ICW3_MASTER                (1 << 2)
#define PIC_SLAVE_IDENTIFICATION_CODE  0x2

#define PIC_ICW4                       0x1

enum pic_interrupts {
	PIC_INTERNAL_TIMER  = 1 << 0,
	PIC_KEYBOARD        = 1 << 1,
	PIC_MASTER_TO_SLAVE = 1 << 2,
	PIC_SERIAL_PORT_A   = 1 << 3,
	PIC_SERIAL_PORT_B   = 1 << 4,
	PIC_FLOPPY_DISK     = 1 << 6,
	PIC_INTERNAL_RTC    = 1 << 8, // Real Time Clock
	PIC_PS2_MOUSE       = 1 << 12,
	PIC_SATA_PRIMARY    = 1 << 14,
	PIC_SATA_SECONDARY  = 1 << 15
};

void pic_init();

uint16_t pic_get_mask();
void pic_write_mask(uint16_t mask);

void pic_acknowledge(uint32_t interrupt_number);

registers_t *irq_default_handler(registers_t *regs);