#pragma once

#include <stddef.h>

typedef struct _pmm_block {
	struct _pmm_block *next;
} pmm_block_t;

uintptr_t pmm_alloc();
uintptr_t pmm_calloc();
void pmm_free(uintptr_t page);