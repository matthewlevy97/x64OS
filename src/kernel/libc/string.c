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

int strcmp(const char *s1, const char *s2)
{
	char *ptr1, *ptr2, val;

	ptr1 = (char*)s1;
	ptr2 = (char*)s2;

	do {
		val = *ptr1++ - *ptr2++;
	} while(!val && (*ptr1 || *ptr2));

	return val;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
	char *ptr1, *ptr2, val;

	ptr1 = (char*)s1;
	ptr2 = (char*)s2;

	if(!n) return 0;

	do {
		val = *ptr1++ - *ptr2++;
	} while(!val && (*ptr1 || *ptr2) && --n);

	return val;
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