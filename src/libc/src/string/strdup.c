#include <string.h>
#include <stdlib.h>

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