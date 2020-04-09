#include <amd64/cpu.h>
#include <stdint.h>

void kmain(uint64_t multiboot_magic, void *multiboot_data)
{
	cpu_init();
	
	while(1);
	return;
}