#include <stdlib.h>
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

char *strdup(const char *s)
{
	size_t len;
	char *ptr;

	len = strlen(s);
	ptr = malloc(len + 1);

	memcpy(ptr, s, len);
	ptr[len] = '\0';

	return ptr;
}
