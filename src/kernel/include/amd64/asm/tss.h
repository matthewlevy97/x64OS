#pragma once

void tss_init();
void tss_install();

void tss_set_rsp(uint8_t index, void *rsp);