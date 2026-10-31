# mimalloc on Windows XP and Linux (branch `winxp-support`)

Notes on replacing the system allocator with mimalloc. On Windows (mainly the XP i386 llvm-mingw `msvcrt.dll` build)
the CRT is hooked at runtime; on Linux the allocation functions are overridden at link time (see
[Linux](#linux-link-time-override)). Android is excluded (see [Android](#android-excluded)).

## Why

On XP the default CRT heap performs poorly when many threads allocate at once. The first workaround was to switch every
process heap to the Low Fragmentation Heap (`enable_lfh_all_heaps()` in `src/Elpida/EntryPoint/Main.cpp`). The second
was to route all CRT allocations to mimalloc. Measured with properly optimized builds, **mimalloc is ~7.5% faster than
LFH**, so mimalloc is the current choice.

## How it works

### Files

| File | Purpose |
|---|---|
| `src/Mimalloc/` | mimalloc sources (single-file build via `static.c`), MIT license in `LICENSE` |
| `src/Mimalloc/static.c` | includes all of mimalloc; `_WIN32`: hook detours and `mi_init_hooks()`, otherwise: direct `malloc`/`free`/... overrides |
| `src/Mimalloc/CMakeLists.txt` | builds the `mimalloc` OBJECT library (an empty INTERFACE library on Android), links `minhook` on `WIN32` |
| `extern/minhook/` | MinHook (git submodule, static library), used to patch the CRT functions at runtime; added only on `WIN32` by `extern/CMakeLists.txt` |
| `src/Elpida/EntryPoint/CMakeLists.txt` | `elpida-entry-point` links `mimalloc` |
| `src/Elpida/EntryPoint/Main.cpp` | calls `mi_init_hooks()` first thing in `main()` on `_WIN32` |

### Why hooks instead of overriding symbols

`static.c` first defined `malloc`, `free`, etc. directly to override the CRT, mirroring `mimalloc-override.h`. That
version is now the non-Windows (`#else`) branch, see [Linux](#linux-link-time-override). With mingw, `malloc` is imported from `msvcrt.dll`, so
defining it locally only affects code in this module. msvcrt itself and other DLLs keep using the CRT heap, which leads
to memory being freed by the wrong allocator. Hooking patches the functions inside the DLL instead, so **every** caller
is redirected.

### Detours (`__h_*`)

Everything below is inside `#if defined(_WIN32)` in `static.c`.

Every function from `mimalloc-override.h` has a `__h_<name>` detour forwarding to the matching `mi_*` function.

Only the ones that exist on Windows are actually hooked in `mi_init_hooks()`:

- `malloc`, `free`, `calloc`, `realloc`, `strdup`, `wcsdup`
- `_expand`, `_msize`, `_recalloc`, `_strdup`, `_wcsdup`, `_mbsdup`
- `_aligned_malloc`, `_aligned_realloc`, `_aligned_recalloc`, `_aligned_msize`, `_aligned_free`
- `_aligned_offset_malloc`, `_aligned_offset_realloc`, `_aligned_offset_recalloc`

The POSIX-only detours (`posix_memalign`, `aligned_alloc`, `reallocarray`, `memalign`, `valloc`, `pvalloc`,
`malloc_usable_size`, `strndup`, `realpath`, `reallocf`, `cfree`, `malloc_size`, `malloc_good_size`, `reallocarr`,
`_posix_memalign`) exist but are unused, because Windows has no such functions to hook.

`_dupenv_s` and `_wdupenv_s` are not hooked. XP's `msvcrt.dll` does not export them (they arrived in msvcr80), and the
mingw headers only declare them in `crtdbg.h`.

### `mi_hook()`

```c
static int mi_hook(void* local, void* detour, void** original, const char* name)
```

1. `MH_CreateHookApiEx(MI_CRT_MODULE, name, ...)` patches the export by name inside the CRT DLL. `MI_CRT_MODULE` is
   `L"msvcrt"`, or `L"ucrtbase"` when building with `_UCRT`.
2. `MH_ERROR_ALREADY_CREATED` is treated as success, because some exports are aliases of each other (e.g. `strdup` /
   `_strdup`).
3. If the DLL does not export the function (`MH_ERROR_MODULE_NOT_FOUND` / `MH_ERROR_FUNCTION_NOT_FOUND`), the copy
   linked into the exe (`&fun`) is hooked instead. This is the case for functions mingw emulates in `libmsvcrt.a`
   because XP's msvcrt lacks them: `_recalloc`, `_aligned_*` and `_aligned_offset_*`.
4. The hook is queued with `MH_QueueEnableHook`. `mi_init_hooks()` applies all of them at once with `MH_ApplyQueued()`,
   so `malloc` and `free` are never hooked separately.

Macros:

- `MI_HOOK(fun)`: hook without keeping the original.
- `MI_HOOK_ORIG(fun)`: also stores the original (MinHook trampoline) in `__o_<fun>`.

### Pointers mimalloc does not own

**Problem:** the program crashed with `mimalloc: error: mi_free: invalid pointer`. Memory allocated by the CRT
*before* `mi_init_hooks()` ran was later freed through the hooked `free`.

**Fix:** every detour that receives a pointer keeps the original function (`MI_HOOK_ORIG`) and hands foreign pointers
back to it:

- `free`, `_aligned_free`: `if (!mi_cfree(p)) __o_free(p);`. `mi_cfree` frees the pointer only if mimalloc owns it and
  returns false otherwise, in a single page-map lookup. `mi_cfree(NULL)` returns false, and the original `free(NULL)`
  does nothing.
- `realloc`, `_expand`, `_msize`, `_recalloc`, `_aligned_realloc`, `_aligned_recalloc`, `_aligned_msize`,
  `_aligned_offset_realloc`, `_aligned_offset_recalloc`: `if (!MI_OWNS(p)) return __o_<fun>(...);`, where
  `MI_OWNS(p)` is `p == NULL || mi_is_in_heap_region(p)`.

## Linux: link-time override

On Linux no hooks are needed. The `#else` branch of `static.c` defines `malloc`, `free`, `calloc`, `realloc`,
`reallocarray`, `aligned_alloc`, `posix_memalign`, `memalign`, `valloc`, `pvalloc`, `malloc_usable_size`, `strdup`,
`strndup`, `realpath`, `wcsdup`, ... directly, forwarding to `mi_*`.

Why this works: ELF executables export symbols that shared libraries reference, and the executable comes first in
symbol lookup. Because `malloc` etc. are defined in the executable, glibc itself, libc++ and every other shared
library call mimalloc. There is no "memory from before the override" problem, so no ownership fallback is needed.

How it gets linked:

- `mimalloc` is an OBJECT library linked directly only by `elpida-entry-point`, so `static.c.o` ends up inside
  `libelpida-entry-point.a`, and from there in every executable that uses the entry point.
- Object libraries are not passed on transitively through `musl-lite`. The benchmark libraries (which link
  `musl-lite`) do **not** get their own `malloc`.
- Being in an archive, `static.c.o` is only pulled in because other code references `malloc`/`free`, which is always
  the case.
- musl's `strdup.c.o`, `strndup.c.o` and `wcsdup.c.o` sit in the same archive. Since `static.c.o` already defines
  those symbols, the musl members are never pulled in and there is no duplicate-symbol error.

Verified on `cmake-build-debug-linux` (Debug, x86_64):

```sh
nm -D apps/Benchmarks/compression-benchmarks | grep -wE "malloc|free|calloc|realloc"   # defined (T) in the exe
nm apps/Benchmarks/compression-benchmarks | grep mi_malloc                               # mimalloc linked in
LD_LIBRARY_PATH=/mnt/Dev/toolchains/llvm-mingw/x86_64-linux-gnu/usr/lib MIMALLOC_SHOW_STATS=1 \
  apps/Benchmarks/compression-benchmarks --index=0 --config=32768                       # runs, prints mimalloc stats
```

Caveat: mimalloc's own override (`alloc/alloc-override.c`, enabled with `MI_MALLOC_OVERRIDE`) also defines
`__libc_malloc`, `__libc_free`, `__libc_memalign`, etc. Old glibc versions (e.g. the dynamic loader allocating TLS
blocks) call these directly, bypassing `malloc`. Modern glibc does not need them, and our hand-written block does not
define them. If `mi_free: invalid pointer` ever appears on an old distro (Ubuntu bionic and older), add those
forwarders, or enable `MI_MALLOC_OVERRIDE` and drop the hand-written block.

## Android: excluded

On Android, `src/Mimalloc/CMakeLists.txt` defines `mimalloc` as an empty INTERFACE library, so Android stays on
bionic's allocator (Scudo on newer versions, jemalloc on older ones).

Why:

- **No process-wide replacement**: the benchmark groups are `MODULE` libraries `dlopen`ed by `Elpida.so`. The
  Android linker searches the global group (`app_process`, bionic `libc.so`) before the library itself, so a
  `malloc` defined in the library is not even used by its own code.
- **`-Bsymbolic` only goes so far**: linking a module with `-Wl,-Bsymbolic` makes its own calls bind to its own
  `malloc`. C++ `new`/`delete` would also need static libc++ (`c++_static`). Two allocators would then coexist in
  the process:
  - bionic memory freed in the module (`getline`, `asprintf`, `realpath(NULL, ...)`, ...) can be handled with the XP
    fallback: `if (!mi_cfree(p)) real_free(p);`, with `real_free` from `dlsym(RTLD_NEXT, "free")`;
  - module memory freed by `Elpida.so` or bionic (e.g. a `unique_ptr<Benchmark>` or result strings created in a
    module and destroyed by the host) reaches bionic's allocator, which aborts. This cannot be fixed from the
    module side.
- **Process-wide alternatives are not viable**: `LD_PRELOAD` via `wrap.sh` only works for debuggable apps, and
  bionic's allocator dispatch is not public API.
- **Less need**: bionic's allocators already use per-thread caches. Android does not have XP's single global heap
  lock.

## Findings

### mimalloc does not use Windows heaps

mimalloc gets all of its memory from `VirtualAlloc` (`prim/windows/prim.c`), never from
`HeapCreate`/`HeapAlloc`. There is no mimalloc heap to switch to LFH. `enable_lfh_all_heaps()` would only affect
allocations that bypass mimalloc (CRT allocations made before the hooks, direct `HeapAlloc` users, MinHook).

### Extra process heap (5 → 6)

With hooks enabled the process has one more heap. It is MinHook's private heap, created in `MH_Initialize()`
(`extern/minhook/src/hook.c`, `HeapCreate(0, 0, 0)`). It is harmless and small. **Do not call `MH_Uninitialize()` at
exit**: it removes the hooks, and later frees of mimalloc memory (atexit handlers, static destructors, CRT teardown)
would go to the CRT `free` and crash.

### Aligned-OS-memory warning

```
mimalloc: warning: unable to allocate aligned OS memory directly, fall back to over-allocation
```

XP has no `VirtualAlloc2`, so mimalloc over-reserves to get aligned arenas. This happens once per arena (stats showed
3 mmaps and 160 MiB reserved for the whole run). It is harmless but costs some 32-bit address space.

### The broken Release build (the real cause of the early slowdown)

mimalloc first measured **slower** than LFH. Its stats printed a `requested` line, which only happens with
`MI_STATS >= 2`, which only happens with `MI_DEBUG > 0`, which means `NDEBUG` was not defined.

The cause was `build/CMakeCache.txt`:

```
CMAKE_BUILD_TYPE:STRING=Release
CMAKE_C_FLAGS_RELEASE:STRING=
CMAKE_CXX_FLAGS_RELEASE:STRING=
CMAKE_ASM_FLAGS_RELEASE:STRING=-O3 -DNDEBUG
```

C and C++ Release flags were empty, so **the whole "Release" build (all benchmarks too) was `-O0` without `NDEBUG`**.
Every earlier measurement, LFH included, was unoptimized. Nothing in the repo sets these to empty. They were likely
passed as `-DCMAKE_C_FLAGS_RELEASE=` by an IDE profile or script, or edited in the cache.

Fixed with:

```sh
cmake -U 'CMAKE_*_FLAGS_RELEASE' build
grep FLAGS_RELEASE build/CMakeCache.txt   # must show -O3 -DNDEBUG for C and CXX
```

Optional safety net, independent of the build type (not currently set; would go in `src/Mimalloc/CMakeLists.txt`):

```cmake
target_compile_definitions(mimalloc PRIVATE MI_DEBUG=0 MI_STATS=0)
```

### Stats from the (unoptimized) run, for reference

- `purges 0`, `resets 0`, `commits 1`, system time 0.6 s of 64.8 s user: decommit/recommit (`purge_delay`) was **not**
  a factor.
- 25 threads (17 peak), 587 abandoned pages, mostly reclaimed: normal.
- ~25 M allocations, 10.3 GiB total. The busiest bins were 32 B, 64 B, 640 B and 3.5 KiB (7.5 GiB alone).

## Licensing

- mimalloc is MIT (`src/Mimalloc/LICENSE`), MinHook is BSD-2-Clause including the Hacker Disassembler Engine
  (`extern/minhook/LICENSE.txt`). Both are compatible with Elpida's GPLv3.
- Binaries must carry their notices: `src/THIRD_PARTY_NOTICES.txt` holds the license texts of all third-party
  code in the shipped binaries, and the root `CMakeLists.txt` installs it with `LICENSE` into `bin`, which is what CI
  packages.
- The file is maintained by hand; versions are locked, so it rarely changes. When a dependency is added or a version
  changes, update its section. Where each part came from:
  - In-repo code (mimalloc, MinHook, musl, zlib, libpng, json.hpp, getopt): the license files and headers in `src/`,
    `extern/` and `include/`.
  - OpenSSL 3.6.0, cpp-httplib 0.14.3, Boost, LLVM 21.1.0, mingw-w64 runtime, winpthreads, GCC Runtime Library
    Exception (gcc 10.5.0): the upstream license files at those versions.
  - Section "Qt 6 third-party components": the SBOMs of the Linux and Windows Qt 6.10.0 builds (build Qt without
    `-no-sbom`). Only `qtbase`, `qtsvg`, `qtcharts` and `qtimageformats` are used, without test-only code, the SQLite
    driver, QtNetwork and build-system files. Each component's license is `PackageLicenseConcluded`; dual licenses use
    the GPLv3-compatible choice (FreeType: FTL, DBus headers: GPL-2.0-or-later, BLAKE2: CC0-1.0). License texts come
    from Qt's `LICENSES/` directories.
  - Section "Shared libraries bundled in the Linux packages": the `.so` files in the Linux Qt and CLI packages, with
    the Debian copyright files of their exact Ubuntu 20.04 package versions (from
    `https://changelogs.ubuntu.com/changelogs/pool/main/...`). libxcb 1.14-2's is not available online, so the 1.15
    one is used (same license).

## Useful runtime knobs

```
MIMALLOC_SHOW_STATS=1           # print stats at exit
MIMALLOC_VERBOSE=0              # quieter output
MIMALLOC_PURGE_DELAY=-1         # never decommit freed memory
MIMALLOC_ARENA_EAGER_COMMIT=1   # commit arenas up front
```

These can also be set in code with `mi_option_set(...)` before `mi_init_hooks()`.

## Next steps

1. **Confirm the 7.5%**: repeat each configuration (mimalloc vs. LFH) several times on the optimized build and compare
   the spread, since run-to-run variance on XP can be a few percent.
2. **Test more machines**: especially higher core counts, where mimalloc should gain more, and at least one low-core
   machine.
3. **Try the runtime knobs**: `MIMALLOC_ARENA_EAGER_COMMIT=1` and `MIMALLOC_PURGE_DELAY=-1`. Little gain is expected
   (no purging was observed), but it is cheap to confirm.
4. **Decide on `MI_DEBUG=0 MI_STATS=0`**: add the line to `src/Mimalloc/CMakeLists.txt` so mimalloc never ends up in
   debug mode because of a broken cache again.
5. **Find what emptied `CMAKE_*_FLAGS_RELEASE`**: check IDE CMake profiles and build scripts so it does not recur.
   Consider a configure-time check that fails when `CMAKE_BUILD_TYPE=Release` and `CMAKE_C_FLAGS_RELEASE` is empty.
6. **Watch for allocator-mismatch crashes**: if msvcrt or another DLL frees memory internally with `HeapFree` instead
   of its exported `free`, mimalloc pointers would reach the Windows heap. Nothing like this has been seen since the
   `MI_OWNS`/`mi_cfree` fallback, but it is the first suspect for any new heap-related crash.
7. **Keep or drop `enable_lfh_all_heaps()`**: it is currently commented out in `main()`. With mimalloc it only affects
   allocations that bypass mimalloc and costs nothing; if kept, call it before `mi_init_hooks()`.
8. **Watch musl duplicates on link changes**: musl's `strdup`/`strndup`/`wcsdup` do not clash today only because they
   share an archive with `static.c.o`. If `mimalloc` objects are ever linked directly next to `musl-lite` objects,
   expect duplicate-symbol errors; drop one side.
9. **Clean up `static.c`**: on Windows, remove the unused POSIX `__h_*` detours (`__h_posix_memalign`, `__h_realpath`,
   ...) and the unhooked `__h__strndup`, `__h__dupenv_s`, `__h__wdupenv_s`. On Linux, remove the Windows-only
   overrides in the `#else` branch (`_expand`, `_msize`, `_recalloc`, `_strdup`, `_aligned_*`, `_dupenv_s`, ...).
10. **Other Windows targets**: decide whether the hooks should also run on x86_64/aarch64 Windows builds, or only on
    XP. Test the `ucrtbase` path (`_UCRT`) if so.
11. **Linux on old distros**: test on the oldest supported distro (bionic). If any `invalid pointer` errors appear, add
    the `__libc_*` forwarders (see [Linux](#linux-link-time-override)).
12. **Linux performance**: compare mimalloc against plain glibc malloc on an optimized Linux build, as was done for XP.
13. **Android (deferred)**: excluded for now (see [Android](#android-excluded)). If revisited, start with one
    self-contained benchmark module linked with `-Bsymbolic`, static libc++ and the `mi_cfree` fallback, and make
    sure nothing it allocates is freed by the host.
