#pragma once

#include <mm/paging.h>

void vmm_set_cr3(page_directory_t directory);
void vmm_reload_cr3();