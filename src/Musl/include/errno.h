#ifndef	_ERRNO_H
#define _ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#include <bits/errno.h>

#ifdef __GNUC__
__attribute__((const))
#endif
#ifdef WIN32
#ifndef _CRT_ERRNO_DEFINED
#define _CRT_ERRNO_DEFINED
__declspec(dllimport) extern int *__cdecl _errno(void);
#define errno (*_errno())
#endif /* _CRT_ERRNO_DEFINED */
#else
int *__errno_location(void);
#define errno (*__errno_location())
#endif

#ifdef _GNU_SOURCE
extern char *program_invocation_short_name, *program_invocation_name;
#endif

#ifdef __cplusplus
}
#endif

#endif

