#include <string.h>

char *strcpy(char *dest, const char *src)
{
	char *save = dest;

	for(; (*dest = *src) != '\0'; ++src, ++dest);

	return save;
}

char *strncpy(char *dest, const char *src, size_t n)
{
	const char *s;
	char *d;
	if(n != 0) {
		d = dest;
		s = src;

		do {
			if((*d++ = *s++) == 0) {
				/* NUL pad the remaining n-1 bytes */
				while(--n != 0)
					*d++ = 0;
				break;
			}
		} while(--n != 0);
	}
	return dest;
}