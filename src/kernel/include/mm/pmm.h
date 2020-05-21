#pragma once

#include <stddef.h>

typedef struct {
	uint64_t number_pages;
	uintptr_t start, end;
} pmm_block_t;

typedef struct {
	uint64_t used_pages;
	uint64_t free_pages;
} pmm_stats_t;

void pmm_init();

uintptr_t pmm_alloc();
uintptr_t pmm_alloc_consecutive_pages(uint64_t pages);
uintptr_t pmm_alloc_region(uintptr_t start_address, uint64_t length);

uintptr_t pmm_calloc();

void pmm_free(uintptr_t page);
void pmm_free_region(uintptr_t start_address, uintptr_t end_address);

void pmm_stats(pmm_stats_t *stats);
void pmm_set_baseline();