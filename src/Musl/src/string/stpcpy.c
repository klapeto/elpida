#include <string.h>

char *__stpcpy(char *restrict d, const char *restrict s)
{
	for (; (*d=*s); s++, d++);

	return d;
}

weak_alias(__stpcpy, stpcpy);
