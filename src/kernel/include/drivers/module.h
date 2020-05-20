#pragma once

// TODO: Placeholder for module information
typedef struct {
	void *ptr;
} module_t;

module_t *module_load(const char *filename);