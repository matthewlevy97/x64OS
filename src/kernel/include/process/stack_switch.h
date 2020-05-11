#pragma once

struct stack_switch_registers
{
  uint64_t rbp;
  uint64_t rbx;
  uint64_t r12;
  uint64_t r13;
  uint64_t r14;
  uint64_t r15;
  uint64_t rbp2;
  uint64_t ret;
};

void stack_switch(void *old_stack_ptr, void *new_stack_ptr, void *page_directory);