#pragma once

#include <mm/paging.h>
#include <stddef.h>

#define PROCESS_NAME_LENGTH 128
#define PROCESS_MAX_THREAD_COUNT 16

typedef enum {
	PROCESS_RUNNING,
	PROCESS_IDLE,
} PROCESS_STATE;

typedef struct {
	uint64_t pid, ppid;
	char name[PROCESS_NAME_LENGTH];
	bool kernel_mode;
	
	PROCESS_STATE state;
	int exit_value;
	
	page_directory_t page_directory;
	
	void (*entry_point)(void);
	
	uint64_t tid, next_tid;
	void *stack_pointer;
	void *tss_stack_pointer;
} process_t;

process_t *process_create(process_t *parent_proc, const char *process_name, bool user, void (*entry_point)(void));

void dump_process(process_t *process);