#include <stdlib.h>

#ifndef __IS_LIBK

void *malloc(size_t size)
{
	// TODO: Userland malloc() implementation
	return NULL;
}

void *calloc(size_t nmemb, size_t size)
{
	// TODO: Userland calloc() implementation
	return NULL;
}

void *realloc(void *ptr, size_t size)
{
	// TODO: Userland realloc() implementation
	return NULL;
}

void free(void *ptr)
{
	// TODO: Userland free() implementation
}

#endif