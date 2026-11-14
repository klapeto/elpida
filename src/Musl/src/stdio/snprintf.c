#include <stdio.h>
#include <stdarg.h>

int snprintf(char *restrict s, size_t n, const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vsnprintf(s, n, fmt, ap);
	va_end(ap);
	return ret;
}


#ifdef _WIN32
// Elpida: see vsnprintf.c
extern __typeof(snprintf) __mingw_snprintf __attribute__((__alias__("snprintf")));
#endif
