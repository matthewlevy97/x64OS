#pragma once

#include <mm/paging.h>
#include <stdint.h>

struct stack_switch_registers
{
  uint64_t r15;
  uint64_t r14;
  uint64_t r13;
  uint64_t r12;
  uint64_t rbp;
  uint64_t rbx;
  uint64_t ret;
};

void process_entry_trampoline();
void process_entry_trampoline_no_elf_load();

void driver_entry_trampoline();
void driver_entry_trampoline_no_elf_load();

void entry_SYSCALL_64();

void do_context_switch(uintptr_t *old_stack_ptr, uintptr_t new_stack_ptr, page_directory_t page_directory);