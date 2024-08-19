#ifndef _STDLIB_H
#define _STDLIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#if __cplusplus >= 201103L
#define NULL nullptr
#elif defined(__cplusplus)
#define NULL 0L
#else
#define NULL ((void*)0)
#endif

#define __NEED_size_t
#define __NEED_wchar_t

#include <bits/alltypes.h>

int atoi (const char *);
long atol (const char *);
long long atoll (const char *);
double atof (const char *);

float strtof (const char *__restrict, char **__restrict);
double strtod (const char *__restrict, char **__restrict);
long double strtold (const char *__restrict, char **__restrict);

long strtol (const char *__restrict, char **__restrict, int);
unsigned long strtoul (const char *__restrict, char **__restrict, int);
long long strtoll (const char *__restrict, char **__restrict, int);
unsigned long long strtoull (const char *__restrict, char **__restrict, int);

int rand (void);
void srand (unsigned);

void *bsearch (const void *, const void *, size_t, size_t, int (*)(const void *, const void *));
void qsort (void *, size_t, size_t, int (*)(const void *, const void *));

int abs (int);
long labs (long);
long long llabs (long long);

typedef struct { int quot, rem; } div_t;
typedef struct { long quot, rem; } ldiv_t;
typedef struct { long long quot, rem; } lldiv_t;

div_t div (int, int);
ldiv_t ldiv (long, long);
lldiv_t lldiv (long long, long long);

int mblen (const char *, size_t);
int mbtowc (wchar_t *__restrict, const char *__restrict, size_t);
int wctomb (char *, wchar_t);
size_t mbstowcs (wchar_t *__restrict, const char *__restrict, size_t);
size_t wcstombs (char *__restrict, const wchar_t *__restrict, size_t);

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define RAND_MAX (0x7fffffff)

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) \
 || defined(_BSD_SOURCE)

#define WNOHANG    1
#define WUNTRACED  2

#define WEXITSTATUS(s) (((s) & 0xff00) >> 8)
#define WTERMSIG(s) ((s) & 0x7f)
#define WSTOPSIG(s) WEXITSTATUS(s)
#define WIFEXITED(s) (!WTERMSIG(s))
#define WIFSTOPPED(s) ((short)((((s)&0xffff)*0x10001U)>>8) > 0x7f00)
#define WIFSIGNALED(s) (((s)&0xffff)-1U < 0xffu)

int rand_r (unsigned *);

#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define WCOREDUMP(s) ((s) & 0x80)
#define WIFCONTINUED(s) ((s) == 0xffff)
void qsort_r (void *, size_t, size_t, int (*)(const void *, const void *, void *), void *);
#endif

typedef int (*cmpfun_q)(const void *, const void *, void *);
hidden void __qsort_r(void *base, size_t nel, size_t width, cmpfun_q cmp, void *arg);

#if defined(_LARGEFILE64_SOURCE)
#define mkstemp64 mkstemp
#define mkostemp64 mkostemp
#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define mkstemps64 mkstemps
#define mkostemps64 mkostemps
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
