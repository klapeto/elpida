/* ----------------------------------------------------------------------------
Copyright (c) 2018-2026, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

// --------------------------------------------------------------------------
// This is a "single file include" of the entire mimalloc library.
// Just include this file in your build to link statically with mimalloc.
// This will also override the standard library allocation functions
// (if cmake -DMI_OVERRIDE=ON (or the macro MI_MALLOC_OVERRIDE=1 is defined)).
//
// This is also used for a static override where we create a single object
// file containing the whole library.
// If such object file is linked first it will override all the standard library
// allocation functions (on Unix's).
// --------------------------------------------------------------------------

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif
#if defined(__sun)
// same remarks as prim/os.c for the static's context.
#undef _XOPEN_SOURCE
#undef _POSIX_C_SOURCE
#endif

#include "mimalloc.h"

#include "prim/os.c"
#include "prim/prim.c"            // includes platform specific prim/<platform>/prim.c
#include "prim/prim-tls.c"
#if MI_OSX_ZONE
#include "prim/osx/alloc-override-zone.c"
#endif

#include "util/libc.c"
#include "util/options.c"
#include "util/random.c"
#include "util/stats.c"
#include "util/threadlocal.c"

#include "arena/arena.c"
#include "arena/arena-alloc.c"
#include "arena/arena-page.c"
#include "arena/bitmap-chunk.c"
#include "arena/bitmap.c"

#include "alloc/alloc.c"           // includes alloc-override.c and free.c (for aliasing to work)
#include "alloc/alloc-generic.c"
#include "alloc/alloc-aligned.c"
#include "alloc/alloc-posix.c"
#include "alloc/page.c"
#include "alloc/page-queue.c"

#include "heap/heap.c"
#include "heap/init.c"
#include "heap/page-map.c"
#include "heap/subproc.c"
#include "heap/theap.c"

#include "profile/sample-guarded.c"
#include "profile/sample-profile.c"
#include "profile/pprof.c"

#if defined(_WIN32)
#include <windows.h>

// detours.h redefines ERROR_DYNAMIC_CODE_BLOCKED from winerror.h
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmacro-redefined"
#include "detours.h"
#pragma clang diagnostic pop

#include <stdio.h>
#include <malloc.h>
#include <mbstring.h>

// Original CRT functions (Detours trampolines). Memory allocated by the CRT before the hooks
// were installed is not owned by mimalloc and must be handed back to these.
static void (*__o_free)(void*);
static void* (*__o_realloc)(void*, size_t);
static void* (*__o__expand)(void*, size_t);
static size_t (*__o__msize)(void*);
static void* (*__o__recalloc)(void*, size_t, size_t);
static void* (*__o__aligned_realloc)(void*, size_t, size_t);
static void* (*__o__aligned_recalloc)(void*, size_t, size_t, size_t);
static size_t (*__o__aligned_msize)(void*, size_t, size_t);
static void (*__o__aligned_free)(void*);
static void* (*__o__aligned_offset_realloc)(void*, size_t, size_t, size_t);
static void* (*__o__aligned_offset_recalloc)(void*, size_t, size_t, size_t, size_t);

#define MI_OWNS(p) ((p) == NULL || mi_is_in_heap_region(p))

void* __h_malloc(size_t size)
{
	return mi_malloc(size);
}

void __h_free(void* p)
{
	if (!mi_cfree(p)) __o_free(p);
}

void* __h_calloc(size_t count, size_t size)
{
	return mi_calloc(count, size);
}

void* __h_realloc(void* p, size_t newsize)
{
	if (!MI_OWNS(p)) return __o_realloc(p, newsize);
	return mi_realloc(p, newsize);
}

void* __h_reallocarray(void* p, size_t count, size_t size)
{
	return mi_reallocarray(p, count, size);
}

void* __h_aligned_alloc(size_t alignment, size_t size)
{
	return mi_aligned_alloc(alignment, size);
}

int __h_posix_memalign(void** p, size_t alignment, size_t size)
{
	return mi_posix_memalign(p, alignment, size);
}

void* __h_memalign(size_t alignment, size_t size)
{
	return mi_memalign(alignment, size);
}

void* __h_valloc(size_t size)
{
	return mi_valloc(size);
}

void* __h_pvalloc(size_t size)
{
	return mi_pvalloc(size);
}

size_t __h_malloc_usable_size(void* p)
{
	return mi_usable_size(p);
}

char* __h_strdup(const char* s)
{
	return mi_strdup(s);
}

char* __h_strndup(const char* s, size_t n)
{
	return mi_strndup(s, n);
}

char* __h_realpath(const char* fname, char* resolved_name)
{
	return mi_realpath(fname, resolved_name);
}

void* __h_reallocf(void* p, size_t newsize)
{
	return mi_reallocf(p, newsize);
}

size_t __h_malloc_size(const void* p)
{
	return mi_usable_size(p);
}

size_t __h_malloc_good_size(size_t size)
{
	return mi_malloc_good_size(size);
}

void __h_cfree(void* p)
{
	mi_free(p);
}

int __h_reallocarr(void* p, size_t count, size_t size)
{
	return mi_reallocarr(p, count, size);
}

int __h__posix_memalign(void** p, size_t alignment, size_t size)
{
	return mi_posix_memalign(p, alignment, size);
}

wchar_t* __h_wcsdup(const wchar_t* s)
{
	return mi_wcsdup(s);
}

void* __h__expand(void* p, size_t newsize)
{
	if (!MI_OWNS(p)) return __o__expand(p, newsize);
	return mi_expand(p, newsize);
}

size_t __h__msize(void* p)
{
	if (!MI_OWNS(p)) return __o__msize(p);
	return mi_usable_size(p);
}

void* __h__recalloc(void* p, size_t count, size_t size)
{
	if (!MI_OWNS(p)) return __o__recalloc(p, count, size);
	return mi_recalloc(p, count, size);
}

char* __h__strdup(const char* s)
{
	return mi_strdup(s);
}

char* __h__strndup(const char* s, size_t n)
{
	return mi_strndup(s, n);
}

wchar_t* __h__wcsdup(const wchar_t* s)
{
	return mi_wcsdup(s);
}

unsigned char* __h__mbsdup(const unsigned char* s)
{
	return mi_mbsdup(s);
}

int __h__dupenv_s(char** buf, size_t* size, const char* name)
{
	return mi_dupenv_s(buf, size, name);
}

int __h__wdupenv_s(wchar_t** buf, size_t* size, const wchar_t* name)
{
	return mi_wdupenv_s(buf, size, name);
}

void* __h__aligned_malloc(size_t size, size_t alignment)
{
	return mi_malloc_aligned(size, alignment);
}

void* __h__aligned_realloc(void* p, size_t newsize, size_t alignment)
{
	if (!MI_OWNS(p)) return __o__aligned_realloc(p, newsize, alignment);
	return mi_realloc_aligned(p, newsize, alignment);
}

void* __h__aligned_recalloc(void* p, size_t count, size_t size, size_t alignment)
{
	if (!MI_OWNS(p)) return __o__aligned_recalloc(p, count, size, alignment);
	return mi_aligned_recalloc(p, count, size, alignment);
}

size_t __h__aligned_msize(void* p, size_t alignment, size_t offset)
{
	if (!MI_OWNS(p)) return __o__aligned_msize(p, alignment, offset);
	(void)alignment;
	(void)offset;
	return mi_usable_size(p);
}

void __h__aligned_free(void* p)
{
	if (!mi_cfree(p)) __o__aligned_free(p);
}

void* __h__aligned_offset_malloc(size_t size, size_t alignment, size_t offset)
{
	return mi_malloc_aligned_at(size, alignment, offset);
}

void* __h__aligned_offset_realloc(void* p, size_t newsize, size_t alignment, size_t offset)
{
	if (!MI_OWNS(p)) return __o__aligned_offset_realloc(p, newsize, alignment, offset);
	return mi_realloc_aligned_at(p, newsize, alignment, offset);
}

void* __h__aligned_offset_recalloc(void* p, size_t count, size_t size, size_t alignment, size_t offset)
{
	if (!MI_OWNS(p)) return __o__aligned_offset_recalloc(p, count, size, alignment, offset);
	return mi_recalloc_aligned_at(p, count, size, alignment, offset);
}

#if defined(_UCRT)
#define MI_CRT_MODULE L"ucrtbase"
#else
#define MI_CRT_MODULE L"msvcrt"
#endif

#define MI_MAX_HOOKS 32

typedef struct mi_hook_s
{
	void* target;
	void** pointer;
} mi_hook_t;

typedef struct mi_hook_alias_s
{
	void** from;
	void** to;
} mi_hook_alias_t;

static mi_hook_t mi_hooks[MI_MAX_HOOKS];
static mi_hook_alias_t mi_hook_aliases[MI_MAX_HOOKS];
static int mi_hook_count;
static int mi_hook_alias_count;

// Detours writes the trampoline into the attached pointer on commit, so hooks that do not keep the
// original still need storage that outlives mi_init_hooks().
static void* mi_hook_unused_originals[MI_MAX_HOOKS];

// Hooks the function exported by the CRT dll so every caller (including the CRT itself and other
// modules) is redirected. Functions the dll does not export (eg. emulated by mingw) are hooked
// at the implementation linked into this module instead.
static int mi_hook(void* local, void* detour, void** original, const char* name)
{
	void* target = (void*)GetProcAddress(GetModuleHandleW(MI_CRT_MODULE), name);
	if (target == NULL)
	{
		target = local;
	}

	// resolve the same way DetourAttach does, so aliases are recognized
	target = DetourCodeFromPointer(target, NULL);

	for (int i = 0; i < mi_hook_count; ++i)
	{
		if (mi_hooks[i].target == target)
		{
			// export is an alias of an already hooked function
			if (original != NULL)
			{
				mi_hook_aliases[mi_hook_alias_count++] = (mi_hook_alias_t){ mi_hooks[i].pointer, original };
			}
			return 0;
		}
	}

	if (mi_hook_count == MI_MAX_HOOKS)
	{
		printf("Failed to hook %s: too many hooks.\n", name);
		return 1;
	}

	void** pointer = original != NULL ? original : &mi_hook_unused_originals[mi_hook_count];
	*pointer = target;

	LONG error = DetourAttach(pointer, detour);
	if (error != NO_ERROR)
	{
		printf("Failed to hook %s: %ld.\n", name, error);
		return 1;
	}

	mi_hooks[mi_hook_count++] = (mi_hook_t){ target, pointer };
	return 0;
}

#define MI_HOOK(fun) if (mi_hook((void*)&fun, (void*)&__h_##fun, NULL, #fun)) goto abort
#define MI_HOOK_ORIG(fun) if (mi_hook((void*)&fun, (void*)&__h_##fun, (void**)&__o_##fun, #fun)) goto abort

int mi_init_hooks()
{
	LONG error = DetourTransactionBegin();
	if (error != NO_ERROR)
	{
		printf("Failed to begin hook transaction: %ld.\n", error);
		return 1;
	}

	MI_HOOK(malloc);
	MI_HOOK_ORIG(free);
	MI_HOOK(calloc);
	MI_HOOK_ORIG(realloc);
	MI_HOOK(strdup);
	MI_HOOK(wcsdup);
	MI_HOOK_ORIG(_expand);
	MI_HOOK_ORIG(_msize);
	MI_HOOK_ORIG(_recalloc);
	MI_HOOK(_strdup);
	MI_HOOK(_wcsdup);
	MI_HOOK(_mbsdup);
	MI_HOOK(_aligned_malloc);
	MI_HOOK_ORIG(_aligned_realloc);
	MI_HOOK_ORIG(_aligned_recalloc);
	MI_HOOK_ORIG(_aligned_msize);
	MI_HOOK_ORIG(_aligned_free);
	MI_HOOK(_aligned_offset_malloc);
	MI_HOOK_ORIG(_aligned_offset_realloc);
	MI_HOOK_ORIG(_aligned_offset_recalloc);

	// enable all hooks at once so malloc/free are never mixed between allocators
	error = DetourTransactionCommit();
	if (error != NO_ERROR)
	{
		printf("Failed to apply hooks: %ld.\n", error);
		return 1;
	}

	for (int i = 0; i < mi_hook_alias_count; ++i)
	{
		*mi_hook_aliases[i].to = *mi_hook_aliases[i].from;
	}

	return 0;

abort:
	DetourTransactionAbort();
	return 1;
}
#else // link-time override (also bionic: only linked into executables, which come first in symbol lookup)
void* malloc(size_t size)
{
	return mi_malloc(size);
}

void free(void* p)
{
	mi_free(p);
}

void* calloc(size_t count, size_t size)
{
	return mi_calloc(count, size);
}

void* realloc(void* p, size_t newsize)
{
	return mi_realloc(p, newsize);
}

void* reallocarray(void* p, size_t count, size_t size)
{
	return mi_reallocarray(p, count, size);
}

void* aligned_alloc(size_t alignment, size_t size)
{
	return mi_aligned_alloc(alignment, size);
}

int posix_memalign(void** p, size_t alignment, size_t size)
{
	return mi_posix_memalign(p, alignment, size);
}

void* memalign(size_t alignment, size_t size)
{
	return mi_memalign(alignment, size);
}

void* valloc(size_t size)
{
	return mi_valloc(size);
}

void* pvalloc(size_t size)
{
	return mi_pvalloc(size);
}

#if defined(__BIONIC__)
size_t malloc_usable_size(const void* p) // bionic declares it const
#else
size_t malloc_usable_size(void* p)
#endif
{
	return mi_usable_size(p);
}

char* strdup(const char* s)
{
	return mi_strdup(s);
}

char* strndup(const char* s, size_t n)
{
	return mi_strndup(s, n);
}

// No realpath() override: mi_realpath() calls realpath() itself, so overriding it recurses forever. libc's
// realpath(NULL) already allocates through the overridden malloc.

void* reallocf(void* p, size_t newsize)
{
	return mi_reallocf(p, newsize);
}

size_t malloc_size(const void* p)
{
	return mi_usable_size(p);
}

size_t malloc_good_size(size_t size)
{
	return mi_malloc_good_size(size);
}

void cfree(void* p)
{
	mi_free(p);
}

int reallocarr(void* p, size_t count, size_t size)
{
	return mi_reallocarr(p, count, size);
}

int _posix_memalign(void** p, size_t alignment, size_t size)
{
	return mi_posix_memalign(p, alignment, size);
}

wchar_t* wcsdup(const wchar_t* s)
{
	return mi_wcsdup(s);
}

void* _expand(void* p, size_t newsize)
{
	return mi_expand(p, newsize);
}

size_t _msize(void* p)
{
	return mi_usable_size(p);
}

void* _recalloc(void* p, size_t count, size_t size)
{
	return mi_recalloc(p, count, size);
}

char* _strdup(const char* s)
{
	return mi_strdup(s);
}

char* _strndup(const char* s, size_t n)
{
	return mi_strndup(s, n);
}

wchar_t* _wcsdup(const wchar_t* s)
{
	return mi_wcsdup(s);
}

unsigned char* _mbsdup(const unsigned char* s)
{
	return mi_mbsdup(s);
}

int _dupenv_s(char** buf, size_t* size, const char* name)
{
	return mi_dupenv_s(buf, size, name);
}

int _wdupenv_s(wchar_t** buf, size_t* size, const wchar_t* name)
{
	return mi_wdupenv_s(buf, size, name);
}

void* _aligned_malloc(size_t size, size_t alignment)
{
	return mi_malloc_aligned(size, alignment);
}

void* _aligned_realloc(void* p, size_t newsize, size_t alignment)
{
	return mi_realloc_aligned(p, newsize, alignment);
}

void* _aligned_recalloc(void* p, size_t count, size_t size, size_t alignment)
{
	return mi_aligned_recalloc(p, count, size, alignment);
}

size_t _aligned_msize(void* p, size_t alignment, size_t offset)
{
	(void)alignment;
	(void)offset;
	return mi_usable_size(p);
}

void _aligned_free(void* p)
{
	mi_free(p);
}

void* _aligned_offset_malloc(size_t size, size_t alignment, size_t offset)
{
	return mi_malloc_aligned_at(size, alignment, offset);
}

void* _aligned_offset_realloc(void* p, size_t newsize, size_t alignment, size_t offset)
{
	return mi_realloc_aligned_at(p, newsize, alignment, offset);
}

void* _aligned_offset_recalloc(void* p, size_t count, size_t size, size_t alignment, size_t offset)
{
	return mi_recalloc_aligned_at(p, count, size, alignment, offset);
}
#endif
