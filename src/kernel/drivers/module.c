#include <boot/multiboot2.h>
#include <drivers/module.h>
#include <kernel/debug.h>
#include <mm/paging.h>
#include <string.h>

uint64_t module_load(module_t *module, const char *filename)
{
	multiboot2_module_tag_t *module_tag;

	if(!module) {
		debug_warning("module.c::module_load(NULL, \"%s\")", filename);
		return 0;
	}

	module->size = 0;

	module_tag = (multiboot2_module_tag_t*)multiboot2_get_tag(MBOOT2_MODULE);
	while(module_tag) {
		if(!strncmp(filename, module_tag->cmdline, strlen(filename))) {
			module->base_addr = (void*)P2V(module_tag->mod_start);
			module->size      = (module_tag->mod_end - module_tag->mod_start);
			break;
		}

		module_tag = (multiboot2_module_tag_t*)multiboot2_get_next_tag(module_tag, MBOOT2_MODULE);
	}

	return module->size;
}