#include <acpi/acpi.h>
#include <amd64/cpu.h>
#include <assert.h>
#include <boot/multiboot2.h>
#include <io/serial.h>
#include <kernel/debug.h>
#include <kernel/kernel.h>
#include <mm/mm.h>
#include <process/process.h>
#include <process/scheduler.h>
#include <stdint.h>

void stage_2()
{
	debug_info("Stage 2 Loaded!\n");

	dump_process(get_current_process());

	debug_info("Kernel loaded!\n");
	while(1);
}

void kmain(uint64_t multiboot_magic, void *multiboot_data)
{
	serial_init();
	
	if(!multiboot2_init(multiboot_magic, multiboot_data))
		PANIC("Multiboot2 failed to be initialized\n");

	if(!mm_init())
		PANIC("MM failed to be initialized\n");

	cpu_init();
	debug_info("CPU Initialized\n");

	acpi_init();
	debug_info("ACPI Initialized\n");

	process_t *stage2 = process_create("stage 2", 0, vmm_get_page_dir(), stage_2);
	scheduler_init(stage2);

	while(1);
	return;
}