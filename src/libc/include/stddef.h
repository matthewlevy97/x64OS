#pragma once

#include <stdint.h>

typedef uint64_t size_t;
typedef int64_t  ssize_t;

#define NULL ((void*)0x00)

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))