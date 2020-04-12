#pragma once

#include <stdint.h>

typedef uint64_t size_t;

#define NULL 0x00

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))