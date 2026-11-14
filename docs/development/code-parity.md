# Benchmark code parity

Elpida's scores are only comparable if every platform runs the same code in the timed parts of the benchmarks. This
document describes what can make platforms diverge, the policy, and the build settings that enforce it.

## What can diverge

- **Runtime dispatch**: the same binary runs different code depending on the CPU it runs on (`cpuid`/`getauxval`
  checks, glibc IFUNCs, aarch64 outline atomics). This is the worst case: two machines running the same binary are not
  running the same benchmark.
- **Build-time divergence**: the code differs per target, but is fixed for a given binary. Causes include per-target
  `#ifdef __SSE2__`/`__ARM_NEON` paths, different compiler defaults (math-errno, FP contraction, CPU baseline),
  different libraries (libc, C++ standard library, allocator) and different library versions.
- **Unavoidable**: compiling the same source for a different ISA (x86 vs ARM vs RISC-V, 32 vs 64-bit) or a different
  platform ABI. These are documented, not fixed.

## Policy

The target is **as much comparable final code as possible**: the same machine code wherever the ISA allows, not just
the same source. Android is held to the same standard as every other platform. The one exception is the StdLib group,
see [below](#the-stdlib-benchmarks).

- The same compiler (llvm-mingw clang) and the same libc++ version on every target, Android included. The NDK's own
  clang and libc++ are not used.
- mimalloc as the allocator and musl-lite for string/stdlib/math functions on every target. The platform libc is
  not used in the timed code.
- No runtime CPU dispatch anywhere. No compile-time SIMD `#ifdef`s that differ between targets of the same
  architecture.
- No `-ffast-math`. FP contraction off. math-errno set explicitly.
- A fixed CPU baseline per architecture. Never `-march=native`.
- Fixed seeds and in-house algorithms for generated input (not done yet, see [known-issues.md](known-issues.md)).

### The StdLib benchmarks

The StdLib group (`src/Benchmarks/StdLib`: `memset`, `memchr`, `strtod`, `strtoul`, `sqrt`, `floor`, `ceil`, regex)
measures **the platform's own C library and allocator**, so it is excluded from the musl-lite/mimalloc rule above:

- `stdlib-benchmarks` links `elpida-entry-point-system-libc` (`src/Elpida/EntryPoint/CMakeLists.txt`), which has no
  musl-lite and no mimalloc (and doesn't call `mi_init_hooks()` on Windows). Calls go to glibc (with its IFUNCs),
  the Windows C runtime DLL, or the device's bionic `libc.so`.
- On Windows the measured functions (`strtod`, `strtoul`, `memset`, `memchr`, `ceilf`, `floorf`, `sqrtf`) are taken
  from the C runtime DLL at runtime (`src/Benchmarks/StdLib/PlatformLibc.hpp`/`.cpp`), selected with the environment
  variable **`ELPIDA_STDLIB_CRT`**: `msvcrt` (default) for `msvcrt.dll`, or `ucrt` for `ucrtbase.dll` (built into
  Windows 10+, otherwise installed by an update or a Visual C++ 2015+ redistributable). An unknown value, or a missing
  DLL or function, fails the benchmark with an error instead of measuring something else. Only these functions
  switch: the process (startup, stdio, the allocator, so also regex's allocations) stays on msvcrt. The Elpida app
  starts the benchmark executables as child processes, so they inherit the variable. Results don't record which CRT
  was used.
- Calling through the DLL also bypasses mingw-w64's own implementations, which would otherwise win at link time:
  `libmsvcrt.a` defines `ceilf`, `floorf`, `sqrtf` as static code, and `stdlib.h` turns `strtod` into an inline wrapper
  around its gdtoa. On i386 neither `msvcrt.dll` nor `ucrtbase.dll` exports `ceilf`/`floorf`/`sqrtf` (per the
  toolchain's import libraries), so the float versions call `ceil`/`floor`/`sqrt` with a conversion to double and
  back, as MSVC's x86 `<math.h>` does. Regex is libc++ code; its own calls are not redirected.
- On Linux and Android, where `stdlib-benchmarks` is dynamically linked, `LD_PRELOAD` can substitute other
  implementations of these functions.
- `malloc` is the system allocator.
- On Android it is built in the `app` part (API 21, dynamically linked against the device's `libc.so`), not as a static
  API 29 executable, so it measures the bionic actually on the device. See [android.md](android.md).
- The build option `ELPIDA_ENABLE_STDLIB_BENCHMARKS` builds it separately from `ELPIDA_ENABLE_BENCHMARKS` (all the
  other groups).
- The rest still applies to the task code itself: the same compiler, parity flags and CPU baseline. `std::regex` is
  still our static libc++, because the platform C++ libraries (libstdc++, MSVC's STL) can't be used with this
  toolchain.
- Differences in the code around the calls are expected: platform headers declare the functions with different
  attributes (glibc marks `ceilf` `const`/`nothrow`; bionic doesn't, so the Android `ceil()` loop stores the result to
  memory after every call).
- `scripts/parity/disasm-parity.py` skips these tasks by default (`--exclude`).

## Compiler flags

Set for all C/C++ code in the root `CMakeLists.txt` (`ELPIDA_PARITY_FLAGS`). The same flags are printed by
`scripts/docker/parity-flags.sh <triple>`, which `scripts/docker/build-llvm.sh` uses for the LLVM libraries and the
llvm-mingw build scripts use for the runtimes (`--additional-cflags-script=`, see
[toolchain.md](toolchain.md#runtimes)). Keep the two in sync. They reach OpenSSL's own build through `CMAKE_C_FLAGS`.

| Flag | Why |
|---|---|
| `-fno-builtin` | Calls go to musl-lite instead of being replaced by compiler builtins. (Explicit `__builtin_*` calls, which libc++ `<cmath>` uses, are not affected; see below.) |
| `-fno-math-errno` | clang's default differs per target: on for glibc and mingw, off for Android and musl. musl never sets `errno` in math functions. With it off, `sqrt()` and the like map to the ISA's instruction on every target. |
| `-ffp-contract=off` | Otherwise `a*b+c` is fused into an FMA only on targets that have one (always on aarch64, never on baseline x86). |
| `-fomit-frame-pointer` | clang keeps frame pointers by default on Android (every ABI) and aarch64 Linux, and omits them elsewhere. Without it, every function of the Android x86_64 benchmarks had an extra `push %rbp; mov %rsp,%rbp`. Unwinding uses `.eh_frame`/`.pdata`, so exceptions are not affected. |
| `-mno-outline-atomics` (aarch64) | Otherwise atomics call `__aarch64_*` helpers that choose LSE or LL/SC at runtime. |
| `-ffixed-x18` (aarch64) | x18 is the platform register: reserved on Android and Windows, allocatable on Linux, where it changed register allocation (e.g. in musl's `floatscan`). |
| `-mfix-cortex-a53-835769`, `-Wl,--fix-cortex-a53-843419` (aarch64) | Cortex-A53 errata workarounds. clang enables both by default for Android only: 835769 puts a `nop` between a memory access and a 64-bit multiply-accumulate (14 in mimalloc's page queues alone), 843419 makes the linker patch some `adrp`. Enabled everywhere, since real A53 cores (Raspberry Pi 3, older phones) need them. lld has no 843419 fix for COFF, so Windows aarch64 only gets 835769. |
| `-march=x86-64 -mtune=generic -mno-sse3 -mno-ssse3 -mno-sse4.1 -mno-sse4.2 -mno-popcnt -mno-cx16` (x86_64) | A fixed baseline. The Android triple enables SSE4.2/POPCNT/CX16 by default. |
| `-march=pentium4 -mtune=generic -mno-sse3 -mno-ssse3` (i386) | A fixed baseline (llvm-mingw's default). The Android triple enables SSSE3 by default. |

Not a parity flag, but set globally next to them: `-ffunction-sections -fdata-sections` and `-Wl,--gc-sections`
(executables and shared libraries, ELF and MinGW). The linker drops unreachable functions and data, mostly LLVM/Clang
code in the Compilation benchmark. Linux x86_64, 2026-11-11: compilation-benchmarks 119 → 106 MB, math 2.2 → 1.6 MB,
encryption 7.6 → 5.7 MB. Executed code is unchanged: `disasm-parity.py compare` of links with and without it differed
only in names (see [parity-checks.md](parity-checks.md#limitations)), and every benchmark ran.

Removed: `-ffast-math` from `src/Benchmarks/Math/CMakeLists.txt`. It lets every target reassociate and vectorize
differently.

### CPU baselines

| Architecture | Baseline | Note |
|---|---|---|
| x86_64 | x86-64 (SSE2) | |
| i386 | pentium4 (SSE2) | The i386/XP build requires SSE2 (it already did by default), so Pentium III and Athlon XP class CPUs are not supported. FP arithmetic uses SSE2; float return values still go through x87 (`st0`, i386 calling convention). |
| aarch64 | ARMv8.0 (`generic`) | |
| armv7 | ARMv7-A, VFPv3-D32, NEON | Linux uses the `hard` float ABI and Android uses `softfp`; see [android.md](android.md#armv7-float-abi). |
| riscv64 | `generic-rv64` | |

### `<cmath>` and compiler builtins

libc++ implements `std::floor`, `std::ceil`, `std::sqrt` and friends with `__builtin_floor` and similar
(`include/c++/v1/__math/rounding_functions.h`). `-fno-builtin` does not stop explicit `__builtin_*` calls, so the
compiler can inline or constant-fold them. On x86_64 Linux the `floor()` and `ceil()` benchmarks compiled to empty
loops.

The StdLib math benchmarks therefore call the C functions directly (through `PlatformLibc.hpp`) and keep the
result alive with `Elpida::DoNotOptimize()` (`include/Elpida/Core/Optimization.hpp`). Each loop makes 10000 calls into
the platform's C library (see [The StdLib benchmarks](#the-stdlib-benchmarks)). Use the same pattern for any benchmark
that must measure a library function.

## Compiler builtins library

Every target uses compiler-rt's builtins (`libclang_rt.builtins-<arch>.a`), e.g. for `__mulsc3` (complex float
multiplication, in the FFT task's loop), `__udivdi3` on 32-bit targets and the 128-bit float routines. Linux used to
add `-lgcc` to the link: it came before compiler-rt on the command line, so lld took these functions from the sysroot's
GCC `libgcc.a` instead. It was removed; x86_64 and i386 Linux link and run without it.

## Checking the binaries

`scripts/parity/disasm-parity.py` follows the code every task's `DoRunImpl()` can reach, reports CPU detection and calls
into the platform's libc, and compares two builds function by function. See [parity-checks.md](parity-checks.md).

## Transparent huge pages

mimalloc is built with `MI_DEFAULT_ALLOW_THP=0` (`src/Mimalloc/CMakeLists.txt`), so every benchmark process disables
transparent huge pages for itself (`prctl(PR_SET_THP_DISABLE)`), including memory not allocated through mimalloc.
mimalloc's own default was 0 on Android but 2 on Linux, where it then depended on each machine's
`/sys/kernel/mm/transparent_hugepage/enabled`. Measured on a Linux host in `madvise` mode, memory latency with a 64 MB
working set: 17.9 ns with THP (70 MB on huge pages), 24.2 ns without. Windows uses no large pages by default.

## Summary

| Component | Used by | Status |
|---|---|---|
| musl-lite (`src/Musl`) | string, stdlib and math functions everywhere | Clean: pure C, no asm |
| OpenSSL 3.6.0 (`no-asm`) | Encryption | Clean: no CPU dispatch. `BN_ULONG` is 32/64-bit per target (unavoidable). |
| zlib 1.3.1 | Compression, PNG | Clean |
| libpng 1.6.50 | Image | Clean: `PNG_HARDWARE_OPTIMIZATIONS` is `OFF` |
| boost-regex | Web | Minor: locale/ctype comes from libc++'s per-OS backend |
| libc++ 21.1.8 | everything | Same version everywhere. `<cmath>` builtins handled (see above). `<random>` distributions not handled yet. |
| LLVM/Clang 19.1.7 | Compilation | Clean once rebuilt: pinned tag, parity flags, Lexer SIMD and xxh3 NEON disabled |
| mimalloc v3 | every allocation | Same allocator, TLS model and THP setting (off) everywhere, Android included; environment variables ignored |
| compiler-rt | builtins, atomics | Clean once rebuilt with the parity flags |
| bionic `libc.a` (Android) | linked into the static Android benchmarks | Outline atomics in its locking on aarch64 (not in hot loops) |

Details per library are in [third-party-libraries.md](third-party-libraries.md). Open items are in
[known-issues.md](known-issues.md).
