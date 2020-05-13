#include <stdlib.h>
#include <ctype.h>

int atoi(const char *str)
{
	int ret;

	ret = 0;
	while(isdigit(*str)) {
		ret = (ret * 10) + (*str - '0');
		str++;
	}

	return ret;
}