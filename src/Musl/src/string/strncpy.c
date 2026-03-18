#include <string.h>

char *strncpy(char *restrict d, const char *restrict s, size_t n)
{
	_MuslLite_stpncpy(d, s, n);
	return d;
}
