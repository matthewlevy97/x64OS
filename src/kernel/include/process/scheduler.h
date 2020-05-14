#pragma once

#include <process/process.h>

void scheduler_init();
void scheduler_run();

void scheduler_add_process(process_t *process);

process_t *get_current_process();