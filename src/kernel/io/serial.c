#include <io/serial.h>
#include <io/ports.h>

#include <stdbool.h>

static bool is_transmit_empty();

void serial_init()
{
	outb(COM_PORT1 + 1, 0x00);    // Disable all interrupts
	outb(COM_PORT1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outb(COM_PORT1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	outb(COM_PORT1 + 1, 0x00);    //                  (hi byte)
	outb(COM_PORT1 + 3, 0x03);    // 8 bits, no parity, one stop bit
	outb(COM_PORT1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outb(COM_PORT1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}
 
void serial_write(char c)
{
	while (!is_transmit_empty());
	outb(COM_PORT1, c);
}

static bool is_transmit_empty()
{
	return inb(COM_PORT1 + 5) & 0x20;
}