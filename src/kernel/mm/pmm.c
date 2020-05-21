#include <assert.h>
#include <kernel/atomic.h>
#include <kernel/debug.h>
#include <mm/paging.h>
#include <mm/pmm.h>
#include <string.h>

#define MIN_PHYSICAL_ADDRESS (PAGE_SIZE)

/**
 * TODO: Key problems for PMM
 * - pmm_alloc() not O(1)
 *   - Possibly create "fast stack" of pages meant to be used/freed often
 * - Stack size for pmm can overflow (no bounds checks in place)
 */

static uintptr_t    max_physical_address;
static pmm_block_t  stack[128];
static uint64_t     stack_size;

static uint64_t used_pages;
static uint64_t free_pages;

static void remove_entry(uint32_t i);
static void insert_entry(uintptr_t start, uintptr_t end);

void pmm_init()
{
	stack_size           = 0;
	max_physical_address = 0;

	used_pages = 0;
	free_pages = 0;
}

uintptr_t pmm_alloc()
{
	return pmm_alloc_consecutive_pages(1);
}

uintptr_t pmm_alloc_consecutive_pages(uint64_t pages)
{
	uintptr_t base_address;

	atomic_begin();

	base_address = 0;
	for(uint32_t i = stack_size; i--;) {
		if(stack[i].number_pages >= pages) {
			base_address = stack[i].start;

			stack[i].start += (pages * PAGE_SIZE);
			stack[i].number_pages -= pages;

			if(!stack[i].number_pages)
				remove_entry(i);

			break;
		}
	}
	
	used_pages += pages;
	free_pages -= pages;

	atomic_end();

	return base_address;
}

uintptr_t pmm_alloc_region(uintptr_t start_address, uint64_t length)
{
	uintptr_t old_end;

	if(start_address < MIN_PHYSICAL_ADDRESS)
		return 0;

	start_address = ALIGN_PAGE(start_address);
	length        = ALIGN_PAGE(length);

	atomic_begin();

	for(uint32_t i = stack_size; i--;) {
		if(stack[i].end >= (start_address + length) && stack[i].start <= start_address) {
			if(stack[i].start == start_address) {
				stack[i].start = start_address + length;
				stack[i].number_pages = (stack[i].end - start_address) / PAGE_SIZE;
			} else {
				old_end = stack[i].end;
				stack[i].end = start_address;
				stack[i].number_pages = (start_address - stack[i].start) / PAGE_SIZE;

				insert_entry(start_address + length, old_end);
			}

			if(!(stack[i].number_pages))
				remove_entry(i);

			goto alloc_success;
		}
	}

	atomic_end();
	return 0;

alloc_success:
	used_pages += (length / PAGE_SIZE);
	free_pages -= (length / PAGE_SIZE);

	atomic_end();
	return start_address;
}

uintptr_t pmm_calloc()
{
	uintptr_t page;

	page = pmm_alloc_consecutive_pages(1);
	if(page) {

		memset(P2V(page), 0, PAGE_SIZE);
	}

	return page;
}

void pmm_free(uintptr_t page)
{
	pmm_free_region(page, page + PAGE_SIZE);
}

void pmm_free_region(uintptr_t start_address, uintptr_t end_address)
{
	uint32_t i;

	// Don't allow free'ing of regions below 1 PAGE
	if(start_address < MIN_PHYSICAL_ADDRESS)
		start_address = MIN_PHYSICAL_ADDRESS;

	start_address = ALIGN_PAGE(start_address);
	end_address   = ALIGN_PAGE(end_address);

	if(start_address >= end_address)
		return;

	atomic_begin();

	if(max_physical_address < end_address) {
		max_physical_address = end_address;
	}

	for(i = stack_size; i; i--) {
		if(stack[i].start == end_address)
			stack[i].start = start_address;
		else if(stack[i].end == start_address)
			stack[i].end = end_address;
		else
			continue;

		stack[i].number_pages += ((end_address - start_address) / PAGE_SIZE);
		break;
	}

	if(!i)
		insert_entry(start_address, end_address);
	
	used_pages -= ((end_address - start_address) / PAGE_SIZE);
	free_pages += ((end_address - start_address) / PAGE_SIZE);
	
	atomic_end();
}

void pmm_stats(pmm_stats_t *stats)
{
	// XXX: Does this need to be an atomic function?? Do we need to be that correct?
	atomic_begin();

	stats->used_pages = used_pages;
	stats->free_pages = free_pages;

	atomic_end();
}

void pmm_set_baseline()
{
	atomic_begin();

	used_pages = 0;

	atomic_end();
}

static inline void remove_entry(uint32_t i)
{
	while(i < stack_size) {
		memcpy(&(stack[i]), &(stack[i+1]), sizeof(pmm_block_t));
		i++;
	}

	if(stack_size) stack_size--;
}

static inline void insert_entry(uintptr_t start, uintptr_t end)
{
	uint64_t size;

	size = (end - start) / PAGE_SIZE;
	if(!size) return;

	stack[stack_size].start        = start;
	stack[stack_size].end          = end;
	stack[stack_size].number_pages = size;

	stack_size++;
}