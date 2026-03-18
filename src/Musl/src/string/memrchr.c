#include <string.h>

void *_MuslLite_memrchr(const void *m, int c, size_t n)
{
	const unsigned char *s = m;
	c = (unsigned char)c;
	while (n--) if (s[n]==c) return (void *)(s+n);
	return 0;
}

weak_alias(_MuslLite_memrchr, memrchr);
