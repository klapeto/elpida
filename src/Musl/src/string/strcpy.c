#include <string.h>

char *strcpy(char *restrict dest, const char *restrict src)
{
	_MuslLite_stpcpy(dest, src);
	return dest;
}
