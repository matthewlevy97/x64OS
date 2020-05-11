#include <kernel/debug.h>
#include <mm/maddrs.h>
#include <mm/paging.h>
#include <mm/pmm.h>
#include <process/process.h>
#include <string.h>

static page_directory_t current_p4;

static inline uint64_t *get_p1(uintptr_t virtual_address, page_directory_t p4);
static uint32_t get_p4_index(uintptr_t address);
static uint32_t get_p3_index(uintptr_t address);
static uint32_t get_p2_index(uintptr_t address);
static uint32_t get_p1_index(uintptr_t address);

page_directory_t vmm_get_page_dir()
{
	return current_p4;
}

void vmm_load_page_dir(page_directory_t page_dir)
{
	current_p4 = (page_directory_t)V2P(page_dir);
}

page_directory_t vmm_create_page_dir()
{
	page_directory_t page_dir, p3, current_p3;
	uint32_t current_p3_flags;

	// Clone kernel space
	page_dir = (page_directory_t)P2V(pmm_calloc());
	p3       = (page_directory_t)pmm_calloc();

	current_p3 = (page_directory_t)((page_directory_t)P2V(current_p4))[511];

	memcpy((void*)P2V(p3), (void*)PAGING_GET_PTR_ADDRESS(P2V(current_p3)), PAGE_SIZE);

	current_p3_flags = (((uintptr_t)current_p3) & 0xFF);
	page_dir[511] = ((uintptr_t)p3 | current_p3_flags);

	return (page_directory_t)V2P(page_dir);
}

page_directory_t vmm_clone_current_page_dir()
{
	uint64_t *p4;
	p4 = (uint64_t*)pmm_alloc();

	// XXX: Might want to implement CoW here
	memcpy(P2V(p4), P2V(current_p4), PAGE_SIZE);

	return (page_directory_t)p4;
}

void vmm_map_page(uintptr_t physical_address, uintptr_t virtual_address)
{
	uint64_t *p1;

	p1 = get_p1(virtual_address, current_p4);
	*p1 = physical_address | PAGE_PRESENT | PAGE_WRITE;	
}

void vmm_map_page2(uintptr_t physical_address, uintptr_t virtual_address, uint16_t flags)
{
	uint64_t *p1;

	p1 = get_p1(virtual_address, current_p4);
	*p1 = physical_address | flags;	
}

void vmm_map_page3(page_directory_t page_dir, uintptr_t physical_address, uintptr_t virtual_address, uint16_t flags)
{
	uint64_t *p1;

	p1 = get_p1(virtual_address, page_dir);
	*p1 = physical_address | flags;	
}

void vmm_set_page_flags(uintptr_t virtual_address, uint16_t flags)
{
	uint64_t *p1;

	p1 = get_p1(virtual_address, current_p4);

	// XXX: Should this just OR the flags, or should the flags be cleared and then OR'ed?
	*p1 |= flags;	
}

bool is_virtual_page_present(uintptr_t page)
{
	uint64_t *level;
	uint32_t i;

	level = P2V(current_p4);
	i     = get_p4_index(page);
	if(!(level[i] & PAGE_PRESENT)) return false;

	level = P2V(PAGING_GET_PTR_ADDRESS(level[i]));
	i     = get_p3_index(page);
	if(!(level[i] & PAGE_PRESENT)) return false;

	level = P2V(PAGING_GET_PTR_ADDRESS(level[i]));
	i     = get_p2_index(page);
	if(!(level[i] & PAGE_PRESENT)) return false;

	level = P2V(PAGING_GET_PTR_ADDRESS(level[i]));
	i     = get_p1_index(page);
	if(!(level[i] & PAGE_PRESENT)) return false;

	return true;
}

registers_t *page_fault_handler(registers_t *regs)
{
	PANIC("Page Fault @ 0x%x\nAccessing: 0x%x\n", regs->rip, regs->cr2);
	return regs;
}

/**
 * @brief      Although ugly, it does its job well of dumping mapped memory
 */
void dump_memory(page_directory_t p4)
{
	page_directory_t p3, p2, p1;
	uint64_t base_address;

	p4 = (page_directory_t)PAGING_GET_PTR_ADDRESS(P2V(p4));
	base_address = 0;

	for(uint64_t p4_i = 0; p4_i < ENTRIES_PER_PT; p4_i++) {
		p3 = (page_directory_t)p4[p4_i];
		if(!p3) {
			if(base_address) {
				debug("0x%x - ", base_address);
				base_address = p4_i << 39;
				debug("0x%x\n", base_address);
				base_address = 0;
			}

			continue;
		}

		p3 = (page_directory_t)PAGING_GET_PTR_ADDRESS(P2V(p3));
		for(uint64_t p3_i = 0; p3_i < ENTRIES_PER_PT; p3_i++) {
			p2 = (page_directory_t)p3[p3_i];
			if(!p2) {
				if(base_address) {
					debug("0x%x - ", base_address);
					base_address = p4_i << 39;
					base_address |= p3_i << 30;
					debug("0x%x\n", base_address);
					base_address = 0;
				}

				continue;
			}

			p2 = (page_directory_t)PAGING_GET_PTR_ADDRESS(P2V(p2));
			for(uint64_t p2_i = 0; p2_i < ENTRIES_PER_PT; p2_i++) {
				p1 = (page_directory_t)p2[p2_i];
				if(!p1) {
					if(base_address) {
						debug("0x%x - ", base_address);
						base_address = p4_i << 39;
						base_address |= p3_i << 30;
						base_address |= p2_i << 21;
						debug("0x%x\n", base_address);
						base_address = 0;
					}

					continue;
				}

				p1 = (page_directory_t)PAGING_GET_PTR_ADDRESS(P2V(p1));
				for(uint64_t p1_i = 0; p1_i < ENTRIES_PER_PT; p1_i++) {
					if(!p1[p1_i]) {
						if(base_address) {
							debug("0x%x - ", base_address);
							base_address = p4_i << 39;
							base_address |= p3_i << 30;
							base_address |= p2_i << 21;
							base_address |= p1_i << 12;
							debug("0x%x\n", base_address);
							base_address = 0;
						}

						continue;
					}

					if(!base_address) {
						base_address = p4_i << 39;
						base_address |= p3_i << 30;
						base_address |= p2_i << 21;
						base_address |= p1_i << 12;
					}
				}
			}
		}
	}
}

static inline uint64_t *get_p1(uintptr_t virtual_address, page_directory_t p4)
{
	uint64_t *level;
	level = P2V(p4);

	if(!(level[get_p4_index(virtual_address)])) {
		level[get_p4_index(virtual_address)] = pmm_calloc() | PAGE_PRESENT | PAGE_WRITE;
	}
	level = (uint64_t*)PAGING_GET_PTR_ADDRESS(P2V(level[get_p4_index(virtual_address)]));

	if(!(level[get_p3_index(virtual_address)])) {
		level[get_p3_index(virtual_address)] = pmm_calloc() | PAGE_PRESENT | PAGE_WRITE;
	}
	level = (uint64_t*)PAGING_GET_PTR_ADDRESS(P2V(level[get_p3_index(virtual_address)]));

	if(!(level[get_p2_index(virtual_address)])) {
		level[get_p2_index(virtual_address)] = pmm_calloc() | PAGE_PRESENT | PAGE_WRITE;
	}
	level = (uint64_t*)PAGING_GET_PTR_ADDRESS(P2V(level[get_p2_index(virtual_address)]));

	return &(level[get_p1_index(virtual_address)]);
}

static inline uint32_t get_p4_index(uintptr_t address)
{
	return (address >> 39) & 511;
}
static inline uint32_t get_p3_index(uintptr_t address)
{
	return (address >> 30) & 511;
}
static inline uint32_t get_p2_index(uintptr_t address)
{
	return (address >> 21) & 511;
}
static inline uint32_t get_p1_index(uintptr_t address)
{
	return (address >> 12) & 511;
}