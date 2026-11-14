#include <stdlib.h>
#include "shgetc.h"
#include "floatscan.h"
#include "stdio_impl.h"

static double strtox(const char *s, char **p, int prec)
{
	FILE f;
	sh_fromstring(&f, s);
	shlim(&f, 0);
	double y = _MuslLite_floatscan(&f, prec, 1);
	off_t cnt = shcnt(&f);
	if (p) *p = cnt ? (char *)s + cnt : (char *)s;
	return y;
}

float strtof(const char *restrict s, char **restrict p)
{
	return strtox(s, p, 0);
}

double strtod(const char *restrict s, char **restrict p)
{
	return strtox(s, p, 1);
}

//long double strtold(const char *restrict s, char **restrict p)
//{
//	return strtox(s, p, 2);
//}

#ifdef _WIN32
// Elpida: mingw-w64's stdlib.h redirects strtod() to __strtod() (msvcrt) or __mingw_strtod() (__USE_MINGW_STRTOX),
// both gdtoa in libmingwex. Define those names here so the calls resolve to musl-lite. Strong aliases: libmingwex
// still gets linked for its printf, and a strong definition there would win over a weak one here.
extern __typeof(strtod) __strtod __attribute__((__alias__("strtod")));
extern __typeof(strtod) __mingw_strtod __attribute__((__alias__("strtod")));
extern __typeof(strtof) __mingw_strtof __attribute__((__alias__("strtof")));
#endif
