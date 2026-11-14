#include "stdio_impl.h"
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

struct cookie {
	char *s;
	size_t n;
};

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static size_t sn_write(FILE *f, const unsigned char *s, size_t l)
{
	struct cookie *c = f->cookie;
	size_t k = MIN(c->n, f->wpos - f->wbase);
	if (k) {
		memcpy(c->s, f->wbase, k);
		c->s += k;
		c->n -= k;
	}
	k = MIN(c->n, l);
	if (k) {
		memcpy(c->s, s, k);
		c->s += k;
		c->n -= k;
	}
	*c->s = 0;
	f->wpos = f->wbase = f->buf;
	/* pretend to succeed, even if we discarded extra data */
	return l;
}

int vsnprintf(char *restrict s, size_t n, const char *restrict fmt, va_list ap)
{
	unsigned char buf[1];
	char dummy[1];
	struct cookie c = { .s = n ? s : dummy, .n = n ? n-1 : 0 };
	FILE f = {
		.lbf = EOF,
		.write = sn_write,
		.lock = -1,
		.buf = buf,
		.cookie = &c,
	};

	*c.s = 0;
	return _MuslLite_vfprintf(&f, fmt, ap);
}

#ifdef _WIN32
// Elpida: mingw-w64's stdio.h redirects vsnprintf() to __mingw_vsnprintf() (__USE_MINGW_ANSI_STDIO, always on for
// libc++), libmingwex's printf on top of gdtoa. Strong alias, so that it wins over libmingwex's definition.
extern __typeof(vsnprintf) __mingw_vsnprintf __attribute__((__alias__("vsnprintf")));
#endif
