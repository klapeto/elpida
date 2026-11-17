#include <string.h>
#include <stdint.h>

void *memset(void *dest, int c, size_t n)
{
	unsigned char *s = dest;
	size_t k;

	/* Elpida: 8 to 32 bytes with overlapping 8-byte stores. Builtins turn small
	 * zeroing loops (libc++'s hash table buckets) into memset calls, and the
	 * byte stores below made those slow; real libcs have a fast small path. */
	typedef uint64_t __attribute__((__may_alias__, __aligned__(1))) u64u;
	if (n - 8 <= 24) {
		uint64_t v = (unsigned char)c * 0x0101010101010101ULL;
		*(u64u *)s = v;
		*(u64u *)(s + n - 8) = v;
		if (n > 16) {
			*(u64u *)(s + 8) = v;
			*(u64u *)(s + n - 16) = v;
		}
		return dest;
	}

	/* Fill head and tail with minimal branching. Each
	 * conditional ensures that all the subsequently used
	 * offsets are well-defined and in the dest region. */

	if (!n) return dest;
	s[0] = c;
	s[n-1] = c;
	if (n <= 2) return dest;
	s[1] = c;
	s[2] = c;
	s[n-2] = c;
	s[n-3] = c;
	if (n <= 6) return dest;
	s[3] = c;
	s[n-4] = c;
	if (n <= 8) return dest;

	/* Advance pointer to align it at a 4-byte boundary,
	 * and truncate n to a multiple of 4. The previous code
	 * already took care of any head/tail that get cut off
	 * by the alignment. */

	k = -(uintptr_t)s & 3;
	s += k;
	n -= k;
	n &= -4;

	/* Pure C fallback with no aliasing violations. */
	for (; n; n--, s++) *s = c;

	return dest;
}
