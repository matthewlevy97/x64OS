#include <acpi/acpi.h>
#include <amd64/cpu.h>
#include <assert.h>
#include <boot/multiboot2.h>
#include <fs/initrd/initrd.h>
#include <fs/vfs.h>
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

void kmain(uint64_t multiboot_magic, void *multiboot_data)
{
	/* boot.S disables interrupts, just need to propogate that value to the atomic counter */
	atomic_set_counter(1);

	serial_init();
	
	if(!multiboot2_init(multiboot_magic, multiboot_data))
		PANIC("Multiboot2 failed to be initialized\n");

	if(!mm_init())
		PANIC("MM failed to be initialized\n");

	kmalloc_init(KERNEL_GLOBAL_HEAP_START);
	debug_ok("Global Kmalloc Initialized\n");

	cpu_init();
	debug_ok("CPU Initialized\n");

	vfs_init(&initrd_fs);

	timer_init();

	acpi_init();
	debug_ok("ACPI Initialized\n");

	// atomic_begin() "called" in boot.S
	atomic_end();

	// Must not be in an atomic state at this point
	ASSERT(!is_atomic());

	debug_ok("Kernel Loaded!\n");

	process_t p = process_create(NULL, "./apps/test_app", false);
	scheduler_init(p);

	while(1);
	return;
}