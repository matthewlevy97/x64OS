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