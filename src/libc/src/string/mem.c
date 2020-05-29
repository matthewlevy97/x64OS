#include <string.h>

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

int memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char *ptr1, *ptr2, val;

	ptr1 = (unsigned char*)s1;
	ptr2 = (unsigned char*)s2;

	do {
		val = *ptr1++ - *ptr2++;
	} while(!val && --n);

	return val;
}
