#define _BSD_SOURCE
#include <string.h>
#include <strings.h>

// Elpida: compilers turn memcmp(...) == 0 into bcmp(), e.g. in libc++. Without this the call goes to the platform libc
// (glibc's bcmp is CPU-dispatched with an IFUNC).
int bcmp(const void *vl, const void *vr, size_t n)
{
	return memcmp(vl, vr, n);
}
