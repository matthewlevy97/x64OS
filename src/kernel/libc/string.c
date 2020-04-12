#include <string.h>

size_t strlen(const char *s)
{
	char *ptr;
	size_t len;

	ptr = (char*)s;
	len = 0;
	while(*ptr++) len++;

	return len;
}

void *memset(void *s, int c, size_t n)
{
	char *ptr;

	ptr = s;
	while(n--) {
		*ptr++ = c;
	}

	return s;
}

void *memcpy(void *dest, const void *src, size_t n)
{
	char *d, *s;

	d = dest;
	s = (char*)src;
	while(n--) {
		*d++ = *s++;
	}

	return dest;
}