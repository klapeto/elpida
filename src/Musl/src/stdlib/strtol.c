#include "stdio_impl.h"
#include "intscan.h"
#include "shgetc.h"
#include <inttypes.h>
#include <limits.h>
#include <ctype.h>

static unsigned long long strtox(const char *s, char **p, int base, unsigned long long lim)
{
	FILE f;
	sh_fromstring(&f, s);
	shlim(&f, 0);
	unsigned long long y = _MuslLite_intscan(&f, base, 1, lim);
	if (p) {
		size_t cnt = shcnt(&f);
		*p = (char *)s + cnt;
	}
	return y;
}

unsigned long long strtoull(const char *restrict s, char **restrict p, int base)
{
	return strtox(s, p, base, ULLONG_MAX);
}

long long strtoll(const char *restrict s, char **restrict p, int base)
{
	return strtox(s, p, base, LLONG_MIN);
}

unsigned long strtoul(const char *restrict s, char **restrict p, int base)
{
	return strtox(s, p, base, ULONG_MAX);
}

long strtol(const char *restrict s, char **restrict p, int base)
{
	return strtox(s, p, base, 0UL+LONG_MIN);
}

intmax_t strtoimax(const char *restrict s, char **restrict p, int base)
{
	return strtoll(s, p, base);
}

uintmax_t strtoumax(const char *restrict s, char **restrict p, int base)
{
	return strtoull(s, p, base);
}

weak_alias(strtol, _MuslLite_strtol_internal);
weak_alias(strtoul, _MuslLite_strtoul_internal);
weak_alias(strtoll, _MuslLite_strtoll_internal);
weak_alias(strtoull, _MuslLite_strtoull_internal);
weak_alias(strtoimax, _MuslLite_strtoimax_internal);
weak_alias(strtoumax, _MuslLite_strtoumax_internal);

// Elpida: glibc >= 2.38 headers redirect strtol() and friends to __isoc23_*() whenever C2X/_GNU_SOURCE features are on,
// which is always the case in C++. Define those names too, so the calls resolve to musl-lite and not to the system glibc
// (the difference is only that the C23 versions accept a "0b" prefix).
weak_alias(strtol, __isoc23_strtol);
weak_alias(strtoul, __isoc23_strtoul);
weak_alias(strtoll, __isoc23_strtoll);
weak_alias(strtoull, __isoc23_strtoull);
weak_alias(strtoimax, __isoc23_strtoimax);
weak_alias(strtoumax, __isoc23_strtoumax);

#ifdef _WIN32
// Elpida: libc++'s num_get calls _strtoi64()/_strtoui64() through their import pointers (__imp_ symbols). The i386
// msvcrt.dll (Windows XP) doesn't export them, so mingw-w64 implements them in libmsvcrt.a, in the same objects as
// strtoll/strtoull/strtoimax/strtoumax. Loading those objects for libc++ replaced musl-lite's functions everywhere (the
// first definition wins with --allow-multiple-definition) by mingw-w64's, which call the CRT's isspace()/isdigit(), e.g.
// in the JSON benchmark's number parsing. Define the names and the import pointers here, so those objects are never
// loaded (on the other Windows targets libc++ then uses musl-lite instead of msvcrt.dll's exports, which is also what
// we want). The pointers are named verbatim: __imp_ + the decorated name.
#define MUSL_STR2(x) #x
#define MUSL_STR(x) MUSL_STR2(x)
#define MUSL_IMP(name) __asm__("__imp_" MUSL_STR(__USER_LABEL_PREFIX__) #name)
extern __typeof(strtoll) _strtoi64 __attribute__((__alias__("strtoll")));
extern __typeof(strtoull) _strtoui64 __attribute__((__alias__("strtoull")));
__typeof(strtoll) *const _MuslLite_imp_strtoi64 MUSL_IMP(_strtoi64) = _strtoi64;
__typeof(strtoull) *const _MuslLite_imp_strtoui64 MUSL_IMP(_strtoui64) = _strtoui64;
__typeof(strtoll) *const _MuslLite_imp_strtoll MUSL_IMP(strtoll) = strtoll;
__typeof(strtoull) *const _MuslLite_imp_strtoull MUSL_IMP(strtoull) = strtoull;
__typeof(strtoimax) *const _MuslLite_imp_strtoimax MUSL_IMP(strtoimax) = strtoimax;
__typeof(strtoumax) *const _MuslLite_imp_strtoumax MUSL_IMP(strtoumax) = strtoumax;
#endif
