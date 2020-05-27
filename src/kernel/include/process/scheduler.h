#pragma once

#include <process/process.h>
#include <kernel/timer.h>

#define SCHEDULER_TIME_SLICE_DURATION     50000000 /* 50 ms */
#define SCHEDULER_TIME_BETWEEN_TICKS      HERTZ_TO_NANOSECONDS(TIMER_FREQUENCY_HERTZ)


void scheduler_init();
void scheduler_run();

void scheduler_add_process(process_t process);

process_t get_current_process();