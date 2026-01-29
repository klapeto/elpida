#include <string.h>

char *strchr(const char *s, int c)
{
	char *r = _MuslLite_strchrnul(s, c);
	return *(unsigned char *)r == (unsigned char)c ? r : 0;
}
