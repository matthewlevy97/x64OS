#include <boot/multiboot2.h>
#include <kernel/kernel.h>
#include <mm/mm.h>
#include <mm/paging.h>
#include <mm/pmm.h>
#include <stdbool.h>
#include <string.h>

extern uintptr_t * _kernel_start;
extern uintptr_t * _kernel_end;
extern uint64_t boot_p4[];

static void load_and_map_physical_pages(uintptr_t base, uint32_t length);
static void load_physical_pages(uintptr_t base, uint32_t length);

int mm_init()
{
	multiboot2_mm_entry_tag_t *entries;
	uint32_t number_entries;
	void *mm_tag;

	vmm_load_p4(boot_p4);

	mm_tag = multiboot2_get_tag(MBOOT2_MM);
	if(!mm_tag) {
		goto init_failed;
	}

	multiboot2_get_mm_entries(mm_tag, &entries, &number_entries);
	if(entries == NULL || number_entries == 0) {
		goto init_failed;
	}

	do {
		switch(entries->type) {
		case MBOOT2_MM_AVAILABLE:
			load_and_map_physical_pages(entries->base_addr, entries->length);
			break;
		default:
			load_physical_pages(entries->base_addr, entries->length);
			break;
		}
		
		entries++;
	} while(--number_entries);

	return 1;
init_failed:
	return 0;
}

static void load_and_map_physical_pages(uintptr_t address, uint32_t length)
{
	uintptr_t end, kend;
	
	kend = ALIGN(V2P((uintptr_t)&_kernel_end), PAGE_SIZE);
	if(address < kend)
		address = kend;

	end = ALIGN(address + length, PAGE_SIZE);
	while(address < end) {
		if(!is_virtual_page_present((uintptr_t)P2V(address)))
			vmm_map_page(address, (uintptr_t)P2V(address));

		pmm_free(address);
		address += PAGE_SIZE;
	}
}

static void load_physical_pages(uintptr_t address, uint32_t length)
{
	uintptr_t end;
	
	end = ALIGN(address + length, PAGE_SIZE);
	while(address < end) {
		if(!is_virtual_page_present((uintptr_t)P2V(address)))
			vmm_map_page(address, (uintptr_t)P2V(address));
		address += PAGE_SIZE;
	}
}