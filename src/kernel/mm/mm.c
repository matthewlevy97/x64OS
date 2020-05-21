#include <boot/multiboot2.h>
#include <kernel/atomic.h>
#include <kernel/debug.h>
#include <kernel/kernel.h>
#include <mm/mm.h>
#include <mm/paging.h>
#include <mm/pmm.h>
#include <stdbool.h>
#include <string.h>

extern uintptr_t * _kernel_start;
extern uintptr_t * _kernel_end;
extern uint64_t boot_p4[];

static void protect_kernel();
static void map_physical_to_virtual(uintptr_t start, uintptr_t end);

/**
 * TODO: Make all pages free, set kernel pages, modules, etc. to unfree immediatly
 * NOTE: Above TODO's must be complete before kmalloc_init()
 */

int mm_init()
{
	multiboot2_mm_entry_tag_t *entries;
	uint32_t number_entries;
	void *mm_tag;
	
	atomic_begin();

	vmm_load_page_dir(boot_p4);

	pmm_init();

	mm_tag = multiboot2_get_tag(MBOOT2_MM);
	if(!mm_tag) {
		goto init_failed;
	}

	/**
	 * Get and mark all regions of accessible memory
	 */
	multiboot2_get_mm_entries(mm_tag, &entries, &number_entries);
	if(entries == NULL || number_entries == 0) {
		goto init_failed;
	}

	do {
		switch(entries->type) {
		case MBOOT2_MM_AVAILABLE:
			pmm_free_region(entries->base_addr, entries->base_addr + entries->length);
			break;
		}
		
		entries++;
	} while(--number_entries);

	pmm_set_baseline();
	protect_kernel();

	/**
	 * Run through memory map again to setup "identity" mapping
	 */
	multiboot2_get_mm_entries(mm_tag, &entries, &number_entries);
	if(entries == NULL || number_entries == 0) {
		goto init_failed;
	}

	do {
		map_physical_to_virtual(entries->base_addr, entries->base_addr + entries->length);		
		entries++;
	} while(--number_entries);

	atomic_end();
	return 1;
init_failed:
	atomic_end();
	return 0;
}

static void protect_kernel()
{
	multiboot2_module_tag_t *module_tag;
	uintptr_t kstart, kend, mboot_base_addr;
	uint32_t mboot_length;

	// Kernel code
	kstart = ALIGN(V2P((uintptr_t)&_kernel_start), PAGE_SIZE);
	kend   = ALIGN(V2P((uintptr_t)&_kernel_end), PAGE_SIZE);
	pmm_alloc_region(V2P(kstart), V2P(kend - kstart));

	// Multiboot2 data
	multiboot2_information(&mboot_base_addr, &mboot_length);
	pmm_alloc_region(V2P(mboot_base_addr), V2P(mboot_length));

	// Module code/data
	module_tag = (multiboot2_module_tag_t*)multiboot2_get_tag(MBOOT2_MODULE);
	while(module_tag) {
		pmm_alloc_region(module_tag->mod_start, module_tag->mod_end - module_tag->mod_start);
		module_tag = (multiboot2_module_tag_t*)multiboot2_get_next_tag(module_tag, MBOOT2_MODULE);
	}
}

static inline void map_physical_to_virtual(uintptr_t start, uintptr_t end)
{
	uintptr_t address;

	address = start;
	while(address < end) {
		if(!is_virtual_page_present((uintptr_t)P2V(address)))
			vmm_map_page2(address, (uintptr_t)P2V(address), PAGE_PRESENT | PAGE_WRITE);
		address += PAGE_SIZE;
	}
}