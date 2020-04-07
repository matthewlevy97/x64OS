#pragma once

typedef unsigned long long size_t;

#define NULL 0x00

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))