#pragma once

#include <mm/paging.h>
#include <stdint.h>

#define PROCESS_NAME_LENGTH 128

// TODO: Set initial RFLAGS for process
#define PROCESS_INITIAL_RFLAGS 0

typedef enum {
	PROCESS_RUNNING,
	PROCESS_IDLE,
	PROCESS_SLEEP,
} PROCESS_STATE;

struct __process {
	uint64_t pid, ppid;
	char name[PROCESS_NAME_LENGTH];
	bool kernel_mode;
	
	PROCESS_STATE state;
	int exit_value;
	
	uint64_t atomic_depth;
	
	/**
	 * Linked list for faster accessing of child/sibling processes
	 */
	struct __process *first_child;
	struct __process *sibling;
	
	page_directory_t page_directory;
	
	void (*entry_point)(void);
	
	uint64_t tid, next_tid;
	uintptr_t stack_pointer;
	uintptr_t kernel_stack_pointer;
};
typedef struct __process process_t;

process_t *process_create(process_t *parent_proc, const char *process_name, bool user, void (*entry_point)(void));

void dump_process(process_t *process);