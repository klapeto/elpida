# mimalloc

Replacing the system allocator with mimalloc on every platform. On Windows (mainly the XP i386 llvm-mingw
`msvcrt.dll` build) the CRT is hooked at runtime. On Linux the allocation functions are overridden at link time (see
[Linux](#linux-link-time-override)), and Android uses the same override in its static benchmark executables (see
[Android](#android-link-time-override-in-the-executables)). Using the same allocator everywhere is part of
[benchmark code parity](code-parity.md).

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
| `src/Mimalloc/CMakeLists.txt` | builds the `mimalloc` OBJECT library, links `detours` on `WIN32` |
| `extern/detours/` | Microsoft Detours (git submodule), used to patch the CRT functions at runtime |
| `extern/CMakeLists.txt` | builds the `detours` static library on `WIN32` from `detours.cpp`, `disasm.cpp` and `modules.cpp` (Detours ships no CMake build) |
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

### Why Detours

The hooks were first implemented with MinHook, which only supports x86 and x64. Detours also supports ARM64 (and ARM,
ARM64EC), is MIT licensed and only uses Win32 APIs that exist on XP (`VirtualAlloc`/`VirtualProtect`/`VirtualQuery`,
`FlushInstructionCache`, `Get`/`SetThreadContext`, `GetProcAddress`, `LoadLibraryEx`).

The submodule is pinned to a commit on `main`, not to the `v4.0.1` tag: that tag is from 2018 and later ARM64 fixes
are only on `main`. The sources build unmodified with llvm-mingw; `-fms-extensions` is needed for MSVC integer
suffixes (`ui64`), and the `ERROR_DYNAMIC_CODE_BLOCKED` redefinition warning from `detours.h` is silenced (also around
the include in `static.c`).

### `mi_hook()`

```c
static int mi_hook(void* local, void* detour, void** original, const char* name)
```

1. The export is looked up by name with `GetProcAddress(GetModuleHandleW(MI_CRT_MODULE), name)`, so the code inside
   the CRT DLL is patched. `MI_CRT_MODULE` is `L"msvcrt"`, or `L"ucrtbase"` when building with `_UCRT` (llvm-mingw
   uses msvcrt for both i386 and aarch64).
2. If the DLL does not export the function, the copy linked into the exe (`&fun`) is hooked instead. This is the case
   for functions mingw emulates in `libmsvcrt.a` because XP's msvcrt lacks them: `_recalloc`, `_aligned_*` and
   `_aligned_offset_*`.
3. The target is resolved with `DetourCodeFromPointer` (as `DetourAttach` does) and compared to the already hooked
   ones, because some exports are aliases of each other (e.g. `strdup` / `_strdup`). An alias is skipped; if it wants
   the original, its `__o_<fun>` is copied from the first hook after commit. Detours has no duplicate detection of its
   own: a second attach would overwrite the first patch and leak a trampoline.
4. `DetourAttach` is called inside the transaction started by `mi_init_hooks()`. `DetourTransactionCommit()` applies
   all hooks at once, so `malloc` and `free` are never hooked separately. If any hook fails, the whole transaction is
   aborted and the CRT allocator stays in use: a partially hooked CRT (e.g. `_aligned_malloc` without
   `_aligned_free`) would mix allocators.

Detours writes the trampoline into the attached pointer on commit, not on attach. Every hook therefore needs a pointer
that outlives the call: `__o_<fun>` for `MI_HOOK_ORIG`, a slot in `mi_hook_unused_originals` for `MI_HOOK`.

Macros (both abort the transaction on failure):

- `MI_HOOK(fun)`: hook without keeping the original.
- `MI_HOOK_ORIG(fun)`: also stores the original (Detours trampoline) in `__o_<fun>`.

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
`strndup`, `wcsdup`, ... directly, forwarding to `mi_*`.

**No `realpath` override**: `mi_realpath()` itself calls `realpath()`, so overriding it recursed forever (with tail calls
this is a silent infinite loop, not a crash). Any `realpath(path, NULL)` hung, on Linux as well. libc's `realpath`
already allocates through the overridden `malloc`, so the override is not needed.

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

## Android: link-time override in the executables

Android used to be excluded: the benchmarks were `MODULE` libraries `dlopen`ed into the .NET app process, where a
`malloc` defined in a library cannot replace bionic's (the global group, `app_process` and `libc.so`, comes first).

Now Android runs the same benchmark executables as the desktop apps, packaged as `lib*.so` so they are extracted to
the executable native library directory (see `apps/Elpida.Mobile/Elpida.Mobile/Platforms/Android/Native/CMakeLists.txt`).
They are **statically linked** (built for API 29), so the override is the same as on Linux. In a static link every
reference, including bionic's own, binds to the first definition (the root `--allow-multiple-definition`). The only
bionic difference in `static.c` is that `malloc_usable_size` takes a `const void*`.

### TLS model

mimalloc defaults to `MI_TLS_MODEL_PTHREADS` on Android (`pthread_getspecific()` for the per-thread heap). Linux uses
`MI_TLS_MODEL_LOCAL` (`__thread`). Measured on the emulator, malloc+free(48) costs 4.55 ns with the pthread model and
2.80 ns with `__thread`, the same as Linux (2.85 ns). This showed up as about 5% in XML and JSON parsing.
`src/Mimalloc/CMakeLists.txt` therefore defines `MI_TLS_MODEL_LOCAL` for Android API >= 29.

Below API 29 Android has no ELF TLS in the dynamic loader, so `__thread` becomes emulated TLS. With mimalloc that
crashes: `__emutls_get_address()` allocates the TLS block with `malloc`, which recurses into mimalloc until the stack
overflows. The static executables avoid that: their TLS is set up by the `libc.a` inside them (current bionic), not by
the device's loader. Verified on Android 5.0 (API 21, `Nexus_6` emulator) and Android 17 (API 37, `Pixel_9a`).

Verified with a test program linked with `static.c`: `mi_is_in_heap_region()` was true for memory from `malloc`,
`strdup`, `asprintf`, `realpath(NULL)`, `getline` and `malloc` on another thread, both statically and dynamically
linked.

`libElpida.so`, which the app P/Invokes, and the tools (`elpida-info-dumper`) are built separately for API 21 and never
link mimalloc.

### Code parity differences (accepted)

Upstream mimalloc compiles differently per libc. These are left as they are to keep changes to external code minimal:

- **Thread id.** `__mi_prim_thread_id()` reads bionic's TLS slot 1 on Android and uses the thread pointer itself
  elsewhere. On aarch64 that is one extra load in `mi_free`'s fast path on Android (`mrs` + `ldr` vs `mrs`); on x86_64
  both are one load (`%fs:0x8` vs `%fs:0x0`).
- **Lock size.** `mi_lock_t` wraps `pthread_mutex_t` (48 bytes on glibc aarch64, 40 on bionic aarch64, 24 on glibc
  armv7, 4 on bionic armv7) or `SRWLOCK`/`CRITICAL_SECTION`, so the structures that embed locks have different field
  offsets. The effect is only which fields share a cache line.

The OS layer also differs (`mmap`/`mmap64`, `__errno_location`/`__errno`, NUMA, init and error output), as do the
platform lock and `pthread_atfork` calls themselves.

## Findings

### mimalloc does not use Windows heaps

mimalloc gets all of its memory from `VirtualAlloc` (`prim/windows/prim.c`), never from
`HeapCreate`/`HeapAlloc`. There is no mimalloc heap to switch to LFH. `enable_lfh_all_heaps()` would only affect
allocations that bypass mimalloc (CRT allocations made before the hooks, direct `HeapAlloc` users).

### Never remove the hooks

**Do not detach the hooks at exit** (`DetourDetach`): later frees of mimalloc memory (atexit handlers, static
destructors, CRT teardown) would go to the CRT `free` and crash.

With MinHook the process had one more heap (MinHook's private `HeapCreate` heap). Detours allocates its trampolines
with `VirtualAlloc` and its bookkeeping with `operator new`. The bookkeeping is allocated by the CRT before the commit
and deleted after it through the hooked `free`. This is safe because `__o_free` is set in the same commit, before the
deletes, and `mi_cfree` hands the foreign pointers back to it.

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

- mimalloc is MIT (`src/Mimalloc/LICENSE`), Detours is MIT (`extern/detours/LICENSE`). Both are compatible with
  Elpida's GPLv3.
- Binaries must carry their notices: `src/THIRD_PARTY_NOTICES.txt` holds the license texts of all third-party
  code in the shipped binaries, and the root `CMakeLists.txt` installs it with `LICENSE` into `bin`, which is what CI
  packages.
- The file is maintained by hand; versions are locked, so it rarely changes. When a dependency is added or a version
  changes, update its section. Where each part came from:
  - In-repo code (mimalloc, Detours, musl, zlib, libpng, json.hpp, getopt): the license files and headers in `src/`,
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

mimalloc ignores its `MIMALLOC_*` environment variables (it is built with `MI_NO_GETENV`), so a machine's environment
cannot change the allocator's behaviour. For diagnostics, configure with `-DELPIDA_MIMALLOC_ALLOW_ENVIRONMENT=ON` to
make these work again. Never publish results from such a build.

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
7. **Keep or drop `enable_lfh_all_heaps()`**: it is currently called in `main()`, before `mi_init_hooks()`. With
   mimalloc it only affects allocations that bypass mimalloc and costs nothing; if kept, keep it before
   `mi_init_hooks()`.
8. **Watch musl duplicates on link changes**: musl's `strdup`/`strndup`/`wcsdup` do not clash today only because they
   share an archive with `static.c.o`. If `mimalloc` objects are ever linked directly next to `musl-lite` objects,
   expect duplicate-symbol errors; drop one side.
9. **Clean up `static.c`**: on Windows, remove the unused POSIX `__h_*` detours (`__h_posix_memalign`, `__h_realpath`,
   ...) and the unhooked `__h__strndup`, `__h__dupenv_s`, `__h__wdupenv_s`. On Linux, remove the Windows-only
   overrides in the `#else` branch (`_expand`, `_msize`, `_recalloc`, `_strdup`, `_aligned_*`, `_dupenv_s`, ...).
10. **Test on real hardware**: the Detours port compiles and links for i386 and aarch64 but has not been run yet.
    Check on XP and on Windows on ARM that `mi_init_hooks()` returns 0 and `MIMALLOC_SHOW_STATS=1` shows the
    allocations. Test the `ucrtbase` path (`_UCRT`) if a UCRT build is ever made.
11. **Linux on old distros**: test on the oldest supported distro (bionic). If any `invalid pointer` errors appear, add
    the `__libc_*` forwarders (see [Linux](#linux-link-time-override)).
12. **Linux performance**: compare mimalloc against plain glibc malloc on an optimized Linux build, as was done for XP.
13. **Android**: done, see [Android](#android-link-time-override-in-the-executables).
