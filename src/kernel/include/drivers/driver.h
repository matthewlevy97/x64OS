#pragma once

#include <stdint.h>

typedef struct {
	char name[48];
	
	int64_t (*init)();
	int64_t (*destroy)();
} driver_t;