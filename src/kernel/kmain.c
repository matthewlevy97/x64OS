#include <acpi/acpi.h>
#include <amd64/cpu.h>
#include <assert.h>
#include <boot/multiboot2.h>
#include <io/serial.h>
#include <kernel/atomic.h>
#include <kernel/debug.h>
#include <kernel/kernel.h>
#include <kernel/timer.h>
#include <mm/kmalloc.h>
#include <mm/mm.h>
#include <mm/pmm.h>
#include <process/process.h>
#include <process/scheduler.h>
#include <stdint.h>

void stage_2()
{
	debug_ok("Kernel loaded!\n");

	while(1);
}

void kmain(uint64_t multiboot_magic, void *multiboot_data)
{
	serial_init();
	
	if(!multiboot2_init(multiboot_magic, multiboot_data))
		PANIC("Multiboot2 failed to be initialized\n");

	if(!mm_init())
		PANIC("MM failed to be initialized\n");

	kmalloc_init(KERNEL_GLOBAL_HEAP_START);
	debug_ok("Global Kmalloc Initialized\n");

	cpu_init();
	debug_ok("CPU Initialized\n");

	timer_init();

	acpi_init();
	debug_ok("ACPI Initialized\n");

	// atomic_begin() "called" in boot.S
	atomic_end();

	// Must not be in an atomic state at this point
	ASSERT(!is_atomic());

	scheduler_init(process_create(NULL, "stage2", true, stage_2));

	while(1);
	return;
}