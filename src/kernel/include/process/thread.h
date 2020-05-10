#pragma once

#include <stddef.h>

typedef enum {
	THREAD_RUNNING,
	THREAD_IDLE,	
} THREAD_STATUS;

typedef struct {
	uint64_t tid;
	void * stack_pointer;
	
	THREAD_STATUS state;
} thread_t;

thread_t *thread_create(uint64_t tid, void (*entry_point)(void));