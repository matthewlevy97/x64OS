#pragma once

#include <stddef.h>

int atoi(const char *str);


#ifdef __IS_LIBK

extern void *kmalloc(size_t size);
extern void *kcalloc(size_t nmemb, size_t size);
extern void *krealloc(void *ptr, size_t size);
extern void kfree(void *ptr);

#define malloc(x) kmalloc(x)
#define calloc(x, y) kcalloc((x), (y))
#define realloc(x, y) krealloc((x), (y))
#define free(x) kfree(x)

#else

void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);

void free(void *ptr);

#endif