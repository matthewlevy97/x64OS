#include <acpi/acpi.h>
#include <amd64/cpu.h>
#include <boot/multiboot2.h>
#include <io/serial.h>
#include <kernel/kernel.h>
#include <kernel/printk.h>
#include <mm/mm.h>
#include <stdint.h>

void kmain(uint64_t multiboot_magic, void *multiboot_data)
{
	serial_init();
	
	if(!multiboot2_init(multiboot_magic, multiboot_data))
		PANIC("Multiboot2 failed to be initialized\n");

	if(!mm_init())
		PANIC("MM failed to be initialized\n");

	cpu_init();

	acpi_init();

	printk("Kernel loaded!\n");

	while(1);
	return;
}