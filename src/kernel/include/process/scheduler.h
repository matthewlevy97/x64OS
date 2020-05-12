#pragma once

#include <amd64/amd64.h>
#include <process/process.h>

void scheduler_init();
void scheduler_run();

void scheduler_add_process(process_t *process);

process_t *get_current_process();

registers_t *scheduler_preemptive(registers_t *regs);