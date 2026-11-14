// Elpida: code compiled against mingw-w64's ctype.h calls the ctype functions of msvcrt.dll through their import
// pointers (__imp_ symbols), which are locale-aware calls into the DLL. Define the pointers here so the calls resolve
// to musl-lite (same pattern as _strtoi64 in strtol.c). The pointers are named verbatim: __imp_ + the decorated name.
#ifdef _WIN32
#include <ctype.h>

#define MUSL_STR2(x) #x
#define MUSL_STR(x) MUSL_STR2(x)
#define MUSL_IMP(name) \
	int (*const _MuslLite_imp_##name)(int) __asm__("__imp_" MUSL_STR(__USER_LABEL_PREFIX__) #name) = name

#undef isalnum
#undef isalpha
#undef iscntrl
#undef isdigit
#undef isgraph
#undef islower
#undef isprint
#undef ispunct
#undef isspace
#undef isupper
#undef isxdigit

MUSL_IMP(isalnum);
MUSL_IMP(isalpha);
MUSL_IMP(iscntrl);
MUSL_IMP(isdigit);
MUSL_IMP(isgraph);
MUSL_IMP(islower);
MUSL_IMP(isprint);
MUSL_IMP(ispunct);
MUSL_IMP(isspace);
MUSL_IMP(isupper);
MUSL_IMP(isxdigit);
MUSL_IMP(tolower);
MUSL_IMP(toupper);
#endif
