#pragma once

#include <stdint.h>

typedef struct {
	void    *base_addr;
	uint64_t size;
} module_t;

uint64_t module_load(module_t *module, const char *filename);