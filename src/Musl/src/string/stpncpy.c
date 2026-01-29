#include <string.h>

char *_MuslLite_stpncpy(char *restrict d, const char *restrict s, size_t n)
{
	for (; n && (*d=*s); n--, s++, d++);
	memset(d, 0, n);
	return d;
}

weak_alias(_MuslLite_stpncpy, stpncpy);

