//
// Created by klapeto on 17/8/2024.
//
#include <stdint.h>
#include <stddef.h>

#define __BYTE_ORDER __BYTE_ORDER__
#define __LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__
#define __BIG_ENDIAN __ORDER_BIG_ENDIAN__

void *memcpy(void *restrict dest, const void *restrict src, size_t n)
{
	unsigned char *d = dest;
	const unsigned char *s = src;

#ifdef __GNUC__

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define LS >>
#define RS <<
#else
	#define LS <<
#define RS >>
#endif

	typedef uint32_t __attribute__((__may_alias__)) u32;
	uint32_t w, x;

	for (; (uintptr_t)s % 4 && n; n--) *d++ = *s++;

	if ((uintptr_t)d % 4 == 0) {
		for (; n>=16; s+=16, d+=16, n-=16) {
			*(u32 *)(d+0) = *(u32 *)(s+0);
			*(u32 *)(d+4) = *(u32 *)(s+4);
			*(u32 *)(d+8) = *(u32 *)(s+8);
			*(u32 *)(d+12) = *(u32 *)(s+12);
		}
		if (n&8) {
			*(u32 *)(d+0) = *(u32 *)(s+0);
			*(u32 *)(d+4) = *(u32 *)(s+4);
			d += 8; s += 8;
		}
		if (n&4) {
			*(u32 *)(d+0) = *(u32 *)(s+0);
			d += 4; s += 4;
		}
		if (n&2) {
			*d++ = *s++; *d++ = *s++;
		}
		if (n&1) {
			*d = *s;
		}
		return dest;
	}

	if (n >= 32) switch ((uintptr_t)d % 4) {
		case 1:
			w = *(u32 *)s;
			*d++ = *s++;
			*d++ = *s++;
			*d++ = *s++;
			n -= 3;
			for (; n>=17; s+=16, d+=16, n-=16) {
				x = *(u32 *)(s+1);
				*(u32 *)(d+0) = (w LS 24) | (x RS 8);
				w = *(u32 *)(s+5);
				*(u32 *)(d+4) = (x LS 24) | (w RS 8);
				x = *(u32 *)(s+9);
				*(u32 *)(d+8) = (w LS 24) | (x RS 8);
				w = *(u32 *)(s+13);
				*(u32 *)(d+12) = (x LS 24) | (w RS 8);
			}
			break;
		case 2:
			w = *(u32 *)s;
			*d++ = *s++;
			*d++ = *s++;
			n -= 2;
			for (; n>=18; s+=16, d+=16, n-=16) {
				x = *(u32 *)(s+2);
				*(u32 *)(d+0) = (w LS 16) | (x RS 16);
				w = *(u32 *)(s+6);
				*(u32 *)(d+4) = (x LS 16) | (w RS 16);
				x = *(u32 *)(s+10);
				*(u32 *)(d+8) = (w LS 16) | (x RS 16);
				w = *(u32 *)(s+14);
				*(u32 *)(d+12) = (x LS 16) | (w RS 16);
			}
			break;
		case 3:
			w = *(u32 *)s;
			*d++ = *s++;
			n -= 1;
			for (; n>=19; s+=16, d+=16, n-=16) {
				x = *(u32 *)(s+3);
				*(u32 *)(d+0) = (w LS 8) | (x RS 24);
				w = *(u32 *)(s+7);
				*(u32 *)(d+4) = (x LS 8) | (w RS 24);
				x = *(u32 *)(s+11);
				*(u32 *)(d+8) = (w LS 8) | (x RS 24);
				w = *(u32 *)(s+15);
				*(u32 *)(d+12) = (x LS 8) | (w RS 24);
			}
			break;
		}
	if (n&16) {
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
	}
	if (n&8) {
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
	}
	if (n&4) {
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
	}
	if (n&2) {
		*d++ = *s++; *d++ = *s++;
	}
	if (n&1) {
		*d = *s;
	}
	return dest;
#endif

	for (; n; n--) *d++ = *s++;
	return dest;
}

#ifdef __GNUC__
typedef __attribute__((__may_alias__)) size_t WT;
#define WS (sizeof(WT))
#endif

void *memmove(void *dest, const void *src, size_t n)
{
	char *d = dest;
	const char *s = src;

	if (d==s) return d;
	if ((uintptr_t)s-(uintptr_t)d-n <= -2*n) return memcpy(d, s, n);

	if (d<s) {
#ifdef __GNUC__
		if ((uintptr_t)s % WS == (uintptr_t)d % WS) {
			while ((uintptr_t)d % WS) {
				if (!n--) return dest;
				*d++ = *s++;
			}
			for (; n>=WS; n-=WS, d+=WS, s+=WS) *(WT *)d = *(WT *)s;
		}
#endif
		for (; n; n--) *d++ = *s++;
	} else {
#ifdef __GNUC__
		if ((uintptr_t)s % WS == (uintptr_t)d % WS) {
			while ((uintptr_t)(d+n) % WS) {
				if (!n--) return dest;
				d[n] = s[n];
			}
			while (n>=WS) n-=WS, *(WT *)(d+n) = *(WT *)(s+n);
		}
#endif
		while (n) n--, d[n] = s[n];
	}

	return dest;
}
