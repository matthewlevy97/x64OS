#include <mm/paging.h>
#include <mm/pmm.h>
#include <string.h>

static uint64_t *current_p4;

static inline uint64_t *get_p1(uintptr_t virtual_address);
static uint32_t get_p4_index(uintptr_t address);
static uint32_t get_p3_index(uintptr_t address);
static uint32_t get_p2_index(uintptr_t address);
static uint32_t get_p1_index(uintptr_t address);

void vmm_load_p4(uint64_t *p4)
{
	current_p4 = p4;
}

uint64_t *vmm_clone_p4()
{
	uint64_t *p4;
	p4 = (uint64_t*)pmm_alloc();

	// XXX: Might want to implement CoW here
	memcpy(P2V(p4), current_p4, PAGE_SIZE);

	return p4;
}

void vmm_map_page(uintptr_t physical_address, uintptr_t virtual_address)
{
	uint64_t *p1;

	p1 = get_p1(virtual_address);
	*p1 = physical_address | PAGE_PRESENT | PAGE_WRITE;	
}

void vmm_map_page2(uintptr_t physical_address, uintptr_t virtual_address, uint16_t flags)
{
	uint64_t *p1;

	p1 = get_p1(virtual_address);
	*p1 = physical_address | flags;	
}

void vmm_set_page_flags(uintptr_t virtual_address, uint16_t flags)
{
	uint64_t *p1;

	p1 = get_p1(virtual_address);

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

static inline uint64_t *get_p1(uintptr_t virtual_address)
{
	uint64_t *level;
	level = P2V(current_p4);

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