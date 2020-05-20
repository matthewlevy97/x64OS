#include <boot/multiboot2.h>
#include <drivers/module.h>
#include <kernel/debug.h>
#include <mm/paging.h>
#include <string.h>

module_t *module_load(const char *filename)
{
	multiboot2_module_tag_t *initrd_tag;

	initrd_tag = (multiboot2_module_tag_t*)multiboot2_get_tag(MBOOT2_MODULE);
	while(initrd_tag) {
		if(!strncmp(filename, initrd_tag->cmdline, strlen(filename))) {
			debug_info("Module Found: 0x%x:0x%x\n", initrd_tag->mod_start, initrd_tag->mod_end);
			break;
		}

		initrd_tag = (multiboot2_module_tag_t*)multiboot2_get_next_tag(initrd_tag, MBOOT2_MODULE);
	}

	// TODO: Create module_t and return data

	return NULL;
}