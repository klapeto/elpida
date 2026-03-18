#include <string.h>

char *strrchr(const char *s, int c)
{
	return _MuslLite_memrchr(s, c, strlen(s) + 1);
}
