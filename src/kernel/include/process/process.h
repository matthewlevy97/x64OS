#pragma once

#include <mm/paging.h>
#include <process/thread.h>
#include <stddef.h>

#define PROCESS_NAME_LENGTH 128
#define PROCESS_MAX_THREAD_COUNT 16

typedef enum {
	PROCESS_RUNNING,
	PROCESS_IDLE,
} PROCESS_STATE;

typedef struct {
	uint64_t pid;
	char name[PROCESS_NAME_LENGTH];
	bool user;
	
	PROCESS_STATE state;
	int exit_value;
	
	page_directory_t page_directory;
	void *kernel_stack;
	
	void (*entry_point)(void);
	
	// TODO: Make this a cyclic list so that the scheduler only needs to load task_list[0]
	thread_t *thread_list[PROCESS_MAX_THREAD_COUNT];
	uint64_t tid_counter;
} process_t;

process_t *process_create(const char *process_name, bool user, page_directory_t page_directory, void (*entry_point)(void));

void dump_process(process_t *process);