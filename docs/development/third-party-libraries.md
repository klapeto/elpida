# Third-party libraries

Audit of every library the benchmarks use, for **runtime** CPU dispatch and for code that differs **per target**.
See [code-parity.md](code-parity.md) for the policy and the compiler flags.

## musl-lite (`src/Musl`)

- Pure C sources for `string`, `stdlib`, `math`, `ctype` and parts of `stdio`. There are no `.s`/`.S` files and no
  arch-specific `src/<arch>/` overrides. The only inline asm is the empty FP barrier in `arch/aarch64/fp_arch.h`.
- Compiled with `-ffreestanding -nostdinc` and linked into `elpida-entry-point`, so every benchmark executable except
  `stdlib-benchmarks` gets it ([the StdLib exception](code-parity.md#the-stdlib-benchmarks)).
  With the root `-Wl,--allow-multiple-definition`, musl's `memcpy`, `memset`, `memchr`, `strtod`, `strtoul`, `ceilf`,
  `sqrtf` and so on win over the platform libc. This was verified in the Linux `compression-benchmarks` binary, which
  defines these symbols itself.
- So glibc IFUNCs (AVX2/EVEX `memchr`, `memset`, ...), msvcrt and bionic are **not** used in the timed code. Keep it
  that way.
- Risk: a call the compiler turns into a builtin never reaches musl. See
  [code-parity.md](code-parity.md#cmath-and-compiler-builtins).
- Risk: a platform header renames the function, and musl-lite does not define the new name. Found with
  `scripts/parity/disasm-parity.py` ([parity-checks.md](parity-checks.md)) and fixed by defining the names in musl-lite:
  - glibc >= 2.38 headers turn `strtol`, `strtoul`, `strtoll`, `strtoull` (and the `intmax` versions) into
    `__isoc23_*` in C++ (`_GNU_SOURCE`), so such calls (e.g. in libc++, boost or LLVM) went to glibc.
    `src/stdlib/strtol.c` defines
    the `__isoc23_*` names as weak aliases (glibc is a shared library, so a weak definition in the executable wins).
  - mingw-w64's `stdlib.h` (msvcrt) does `#define strtod __strtod`, and with `__USE_MINGW_STRTOX` calls
    `__mingw_strtod`. Both are gdtoa in libmingwex, so `strtod` calls in our code went there.
    `src/stdlib/strtod.c` defines `__strtod`, `__mingw_strtod` and `__mingw_strtof` as **strong** aliases on Windows,
    because libmingwex still gets linked for its printf.
  - `bcmp` (which compilers emit for `memcmp(...) == 0`, e.g. in libc++) was missing and came from glibc, where it is an
    IFUNC. Added as `src/string/bcmp.c`.
  - (2026-11-12) A platform library that defines a musl-lite function **and** something else that gets linked wins,
    because with `--allow-multiple-definition` the first definition loaded is kept. libc++'s `num_get` imports
    `_strtoi64`/`_strtoui64` (`__imp_` references). The i386 msvcrt.dll (Windows XP) has neither, so mingw-w64's
    `libmsvcrt.a` implements them, in the same objects as `strtoll`, `strtoull`, `strtoimax` and `strtoumax`. Those
    objects were loaded before musl-lite's `strtol.c`, so i386 Windows ran mingw-w64's `strtoull` (calling msvcrt's
    `isspace`/`isdigit`/`isupper`/`islower` per character) in the JSON benchmark's number parsing.
    `src/stdlib/strtol.c` now also defines `_strtoi64`, `_strtoui64` and the `__imp_` pointers of all six names on
    Windows. lld prints `warning: duplicate symbol` for such cases: check a build log for it after changing libraries
    (the only expected ones are `__strtod`/`__mingw_strtod`, where musl-lite's object is listed first).
- `strtod`/`strtof` (`src/internal/floatscan.c`) compute in `double` on every target. Upstream computes in
  `long double`, which is x87 80-bit on x86, IEEE binary128 in software (`__addtf3`, `__multf3`, ...) on aarch64
  Linux/Android and Android x86_64, and 64-bit on Windows arm64 and armv7, so number parsing did different work per
  ABI. This is the code path musl itself uses where `long double` is `double`. `wcstold` is limited to double
  precision; musl-lite has no `strtold`.
  - Verified against glibc's correctly rounded `strtod`/`strtof` on x86_64, i386 and aarch64 (qemu): 25.9 million
    conversions (random bit patterns printed several ways, exact halfway cases, random digit strings, subnormals,
    overflow, hex floats with digits around the rounding point), bit-identical results and end pointers.
  - That test found a bug in `hexfloat` for this path: it kept one hex digit too few (`FS_MANT_DIG/4+1`), so the sticky
    digit landed on the rounding bit and `0x1.000000000000081p0` rounded down. Fixed (`+2`). Upstream musl presumably
    has it on targets where `long double` is `double`; not checked.
  - Linux and Android (x86_64 and aarch64) now run identical `strtod` code apart from the platform errno.
- (2026-11-14) `snprintf`, `vsnprintf`, `sprintf` and `vsprintf` (`src/stdio/vfprintf.c` and friends, from musl
  1.2.5). Before, these were glibc's, mingw-w64's (`__mingw_vsnprintf` → `__mingw_pformat` and gdtoa, linked
  statically) or bionic's, reached from nlohmann's error messages, LLVM's `format`/`write_double` and libc++'s
  `std::to_string`. Changes to upstream:
  - The formatter is internal (`_MuslLite_vfprintf`), only used through vsnprintf's string `FILE`: musl-lite's `FILE`
    is not the platform's, so `vfprintf`/`fprintf`/`printf` on real streams stay the platform's.
  - Floating point is formatted in `double` (`fmt_fp`), like `floatscan`: upstream uses `long double`, a different
    type per target. `%Lf` arguments are converted to double.
  - Fixed `%.12a` (13 hex digits printed instead of rounding to 12): upstream skips the rounding at exactly
    `LDBL_MANT_DIG/4-1` digits, which only matters where `long double` is `double` (so upstream musl has it on those
    targets).
  - No `%m` (it would call the platform's `strerror`) and `%lc`/`%ls` handle ASCII only (`EILSEQ` otherwise), as in
    the C locale of glibc and msvcrt. No locking (the string `FILE` never locks).
  - Windows: strong aliases `__mingw_snprintf`, `__mingw_vsnprintf`, `__mingw_sprintf`, `__mingw_vsprintf` (mingw-w64's
    `stdio.h` redirects to them with `__USE_MINGW_ANSI_STDIO`, which libc++ turns on). No linked code uses the MS-only
    `%I64d` formats (checked in the binaries' strings), which musl doesn't support.
  - Verified against glibc on x86_64 (53M checks), i386, aarch64, armv7 and riscv64 (8.9M each, qemu): `%e/%f/%g/%a`
    with flags, widths and precisions 0-40, 300 and 330 on random bit patterns, decimal-like and scaled values,
    specials and halfway cases, plus integer and string conversions and truncation. Identical output and return
    values, except `%a` of subnormals: glibc prints `0x0.0000000000001p-1022`, musl `0x1p-1074` (both valid).
  - `ecvt`/`fcvt`/`gcvt`, which use `sprintf`, now use musl-lite's too. `include/stdarg.h` and `include/stddef.h` were
    added (upstream musl) for these sources.
- (2026-11-14) ctype: `isalnum` ... `isxdigit`, `tolower`, `toupper` (`src/ctype`, musl 1.2.5 without the `_l`
  variants) and glibc's table functions `__ctype_b_loc`, `__ctype_tolower_loc`, `__ctype_toupper_loc`, which code
  compiled against glibc's `ctype.h` reads directly (LLVM, OpenSSL). On Windows `src/ctype/windows-imports.c`
  defines the `__imp_` pointers, because mingw-w64's headers call msvcrt.dll's locale-aware functions through them.
  - The `tolower`/`toupper` tables hold glibc's values outside the letters (the byte itself; upstream musl has 0),
    so glibc-compiled code gets glibc's results.
  - Verified against glibc's C locale for -1..255, functions and all three tables. For -128..-2 (undefined behaviour)
    the functions return the value itself like msvcrt; glibc's tables map them to the byte (`c + 256`).

## OpenSSL 3.6.0 (`cmake-modules/openssl.cmake`)

- Built with `no-asm no-hw no-engine ...`. `libcrypto.a` still exports `OPENSSL_cpuid_setup` and
  `OPENSSL_ia32cap_P`, but only as the C stub (`cpuid.c` without `OPENSSL_CPUID_OBJ`). There are no
  `aesni_*`/`vpaes_*`/`bsaes_*`/`rsaz_*` symbols. `AES_encrypt` is the portable `aes_core.c` and `bn_mul_mont` is C.
- `BN_ULONG` is 32-bit on i386/armv7 and 64-bit on x86_64/aarch64/riscv64. This is unavoidable: RSA does different
  amounts of word-level work on 32-bit and 64-bit targets.
- Keep `no-asm`. Dropping it brings in AES-NI, ARMv8 Crypto Extensions and AVX-512 `rsaz`, chosen at runtime through
  `OPENSSL_ia32cap`/`OPENSSL_armcap`.

## zlib 1.3.1 (`extern/zlib`)

- madler/zlib: no runtime CPU detection, no SIMD, no asm options in its CMake build. Clean.
- Do not switch to zlib-ng or Chromium's zlib. Both dispatch at runtime (SSE4.2/AVX2/PCLMUL/NEON for CRC and match).
- Note: zlib's CMake renames the source tree's `zconf.h` to `zconf.h.included` on every out-of-source configure, so
  the submodule shows `zconf.h` as deleted. `git checkout -- zconf.h` in `extern/zlib` restores it.

## libpng 1.6.50 (`extern/libpng`)

- `PNG_HARDWARE_OPTIMIZATIONS` is forced `OFF` in `extern/CMakeLists.txt`. Its default is `ON`, which compiles SSE2
  filters on x86_64 (only when `__SSE2__` is defined, so not on every x86 target), NEON on ARM (with runtime detection
  on some targets), and VSX/MSA elsewhere.
- Those optimizations only affect **reading** (row unfiltering). The scored task of `PngEncodingBenchmark` is the
  encoding (`coreTaskIndex = 2`), so scores were not affected yet, but a decode benchmark would have diverged.

## boost-regex (`extern/boost-regex`, standalone)

- C++ with no SIMD or CPU detection.
- Character classification and case folding go through `std::locale`/`ctype` from libc++, whose locale backend is
  per-OS (glibc, msvcrt, bionic). The impact is small for ASCII patterns, but it is not identical code.

## libc++ 21.1.8

All targets use llvm-mingw's `-stdlib=libc++ -rtlib=compiler-rt -unwindlib=libunwind` (`linux-common.cfg`,
`mingw32-common.cfg`), and Android uses the same version, built by the fork's `build-libcxx.sh`. libstdc++, MSVC STL and
the NDK's libc++ are not used.

- **`<cmath>`** uses compiler builtins. See [code-parity.md](code-parity.md#cmath-and-compiler-builtins).
- **`<random>` distributions are implementation-defined.** `std::mt19937` output is specified, but
  `std::normal_distribution`, `std::uniform_int_distribution`, `std::uniform_real_distribution` and `std::shuffle`
  are not. With the same libc++ version everywhere the algorithms are the same, but several tasks also seed from
  `std::random_device`, so the input changes on every run. This doesn't bias scores, but it prevents verifying parity
  with input/output checksums. See [known-issues.md](known-issues.md#non-deterministic-input).
- **`std::regex`** (StdLib Regex) is libc++'s implementation, identical everywhere because the version is the same. Its
  allocations go to the platform allocator, like the rest of the StdLib group.
- Some of libc++ is compiled into `libc++.a` (for example `shared_ptr` reference counting). It has to be built with the
  parity flags. See [toolchain.md](toolchain.md#runtimes).

## LLVM/Clang 19.1.7 (Compilation benchmark)

- The benchmark links the LLVM/Clang 19.1.7 libraries built by `scripts/docker/build-llvm.sh` (the 21.1.8 toolchain
  only compiles Elpida). See [toolchain.md](toolchain.md#llvm-libraries-for-the-compilation-benchmark).
- `CompileWithClangTask` forces `Triple = "x86_64-none-none"` with `-O3`, so every host does the same frontend,
  optimizer and codegen work.
- `clang/lib/Lex/Lexer.cpp` has compile-time SIMD paths: `#ifdef __SSE4_2__` (identifier lexing) and
  `#ifdef __SSE2__`/`__ALTIVEC__` (block comment skipping). x86 used SSE2 there, other architectures the scalar loop.
  `scripts/docker/llvm.parity.patch` (applied by `build-llvm.sh`) disables all of them. The scalar comment loop is
  written to be auto-vectorized, which is ordinary per-ISA compilation.
- `llvm/lib/Support/xxhash.cpp` uses NEON on aarch64, and `StringMap` hashes every identifier with it.
  `build-llvm.sh` passes `-DLLVM_XXH_USE_NEON=0`.
- `llvm/lib/Support/BLAKE3` has real runtime dispatch (SSE2/SSE4.1/AVX2/AVX-512/NEON). It is only used for module
  dependency scanning, MemProf and CodeView type hashes, none of which are on the benchmark path. Keep it that way:
  no `-g`/CodeView in the benchmark invocation.
- Assertions and `LLVM_ENABLE_ABI_BREAKING_CHECKS` must stay off. With them on, `llvm::hash_value` uses a per-process
  seed, so hash table layouts vary between runs.
- TLS: LLVM has a few `thread_local`s (`PrettyStackTraceHead`, `TimeTraceProfilerInstance`, `BottomOfStack`, ...).
  Callgrind counted about 45K accesses per compile, so even emulated TLS would cost about 0.02%.

## mimalloc v3 (`src/Mimalloc`)

- Same allocator on every platform. See [mimalloc.md](mimalloc.md).
- The only CPU detection (`_mi_detect_cpu_features()`, popcnt via `cpuid`) is used only in the `_MSC_VER` branch of
  `mi_popcount`. Under clang, `__builtin_popcount` is used, which is decided at compile time. `util/libc.c` now returns
  early outside MSVC, so the benchmarks execute no `cpuid` in the allocator at all.
- The TLS model is the same everywhere (`__thread`), Android included. See [android.md](android.md#tls).
- mimalloc ignores its `MIMALLOC_*` environment variables (`MI_NO_GETENV`), so a machine's environment cannot change
  its behaviour. `-DELPIDA_MIMALLOC_ALLOW_ENVIRONMENT=ON` re-enables them for diagnostics. See
  [mimalloc.md](mimalloc.md#useful-runtime-knobs).

## Runtime lookups and OS feature probing

Besides CPU checks, code can choose an implementation at runtime by looking for a function or library
(`dlopen`/`dlsym`, `LoadLibrary`/`GetProcAddress`, IFUNC) or by probing the kernel. Checked on the built benchmarks:

- **Linux:** the binaries import `dlopen`/`dlsym` (LLVM's `DynamicLibrary`, for plugins and the JIT; libunwind, for
  exception unwinding), but an `LD_PRELOAD` logging shim recorded **no calls** during the Compilation, XML, JSON and
  Boost regex benchmarks. No IFUNC relocations.
- **Android (static):** no IFUNC symbols or IRELATIVE relocations. bionic's per-CPU string functions (armv7
  `__memcpy_a9`/`_a15`/`_a53`/`_krait`, ...) and their resolvers are linked in but never used: `memcpy`, `memset`,
  `memmove`, `memchr`, `strlen`, `strcmp` and `strchr` are musl's (same sizes as the musl objects) on every ABI.
  compiler-rt's `__init_cpu_features` (aarch64) runs at startup, but only its SME support routines use the result.
- **Windows:** the same 25 APIs are looked up with `GetProcAddress` in every benchmark, from two sources:
  - **mimalloc's Windows layer:** `VirtualAlloc2`/`NtAllocateVirtualMemoryEx` (aligned reservations, Windows 10+),
    NUMA and large-page functions, `RtlDllShutdownInProgress`. This only affects how arenas are reserved from the OS
    (once per arena; on XP mimalloc over-reserves), not the per-allocation path.
  - **hwloc:** `GetLogicalProcessorInformationEx` and processor groups (Windows 7+), `SetThreadGroupAffinity`,
    `VirtualAllocExNuma`. This affects topology detection, thread pinning and NUMA placement (`--numa-aware`), not the
    timed code.
- **mimalloc on Linux/Android** reads `/sys/kernel/mm/transparent_hugepage/*`, can call `prctl(PR_SET_THP_DISABLE)`,
  and falls back from `MADV_FREE` to `MADV_DONTNEED` (only used when `purge_decommits=0`, not the default). The
  transparent huge page default differed per platform; it is now off everywhere, see
  [code-parity.md](code-parity.md#transparent-huge-pages).

## compiler-rt

- Provides the builtins and, on aarch64, the outline-atomics helpers. Our code is built with `-mno-outline-atomics`,
  so it never calls them. The runtimes have to be built with the parity flags too. See
  [toolchain.md](toolchain.md#runtimes).

## bionic (Android)

- The static Android benchmarks link the NDK's `libc.a`. On aarch64 it uses outline atomics in its locking. See
  [android.md](android.md#bionic-outline-atomics-aarch64).
