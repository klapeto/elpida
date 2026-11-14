#include <stdio.h>
#include <limits.h>

int vsprintf(char *restrict s, const char *restrict fmt, va_list ap)
{
	return vsnprintf(s, INT_MAX, fmt, ap);
}

#ifdef _WIN32
// Elpida: see vsnprintf.c
extern __typeof(vsprintf) __mingw_vsprintf __attribute__((__alias__("vsprintf")));
#endif
