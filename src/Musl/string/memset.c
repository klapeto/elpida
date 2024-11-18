#include <string.h>

void *memset(void *dest, int c, size_t n)
{
	unsigned char *s = dest;

	/* Pure C fallback with no aliasing violations. */
	for (; n; n--, s++) *s = c;

	return dest;
}
