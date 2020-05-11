#include <mm/pmm.h>
#include <mm/paging.h>
#include <string.h>
#include <kernel/debug.h>
/**
 * This is a dirty simple Physical Memory Manager.
 * I will eventually create a more efficent one than a linked-list
 * It is initialized by free'ing the first block
 */

static pmm_block_t *tail = NULL;

uintptr_t pmm_alloc()
{
	uintptr_t page;
	if(!tail) {
		// TODO: Should we PANIC() here or let the caller function handle this?
		return 0;
	}

	page = (uintptr_t)tail;
	tail = tail->next;
	
	return V2P(page);
}

uintptr_t pmm_calloc()
{
	uintptr_t page;

	page = pmm_alloc();
	if(page)
		memset(P2V(page), 0, PAGE_SIZE);

	return page;
}

void pmm_free(uintptr_t page)
{
	pmm_block_t *block = P2V(page);
	block->next = tail;
	tail = P2V(block);
}