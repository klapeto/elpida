#include <string.h>

char *_MuslLite_stpcpy(char *restrict d, const char *restrict s)
{
	for (; (*d=*s); s++, d++);

	return d;
}

weak_alias(_MuslLite_stpcpy, stpcpy);
