# Known issues

Open problems and remaining divergences. See [code-parity.md](code-parity.md) for what is already handled.

## Rebuilds pending

- **Linux/Windows runtimes** (compiler-rt, libc++) were built without the parity flags. See
  [toolchain.md](toolchain.md#runtimes). The parity checks show it: `__mulsc3` (FFT) and the 128-bit float routines
  (`__addtf3`, `__multf3`, ... used by `strtod` on aarch64) differ between Linux and Android.
  Measured 2026-11-09 on aarch64 Linux: `libc++.a` has ~1750 frame-pointer setups, uses x18 and calls outline atomics;
  `lib/clang/21/lib/linux/libclang_rt.builtins-aarch64.a` uses x18 and references `__aarch64_cas*`.
- Done 2026-11-09: the Android sysroots (API 21 and 29), their runtimes and the LLVM libraries of every target were
  rebuilt with the current flags. The Android and Linux LLVM libraries now match (same remaining frame-pointer
  functions, the ones that need one).

- **LLVM libraries without `-fno-builtin` (2026-11-16):** `build-llvm.sh` no longer passes it; the libraries in the
  local toolchain was rebuilt on 2026-11-17, the Docker images still have it until rebuilt. The arm64 Windows crash
  behind the flag (musl-lite's 32-bit `wchar_t` there) is fixed 2026-11-18, confirmed on arm64 Windows hardware. See
  [compilation-benchmark.md](compilation-benchmark.md).

## Non-deterministic input

Several tasks generate their input with `std::random_device` seeds, `std::normal_distribution`,
`std::uniform_int_distribution` or `std::shuffle`:

- `Math/MatrixMultiplication32Task.cpp`, `Math/DynamicMatrixMultiplicationTask.cpp`,
  `Math/CalculateFFTInPlaceTask.cpp`: **fixed since 2026-11-14**: `mt19937(12345)` + libc++'s `normal_distribution`,
  so the same input on every run and platform (libc++ is the same everywhere and its `log`/`sqrt` are musl-lite's;
  x86_64 Linux vs Windows differ only in the Windows ABI's `xmm6`+ saves in `log`). A libc++ upgrade may change the
  generated values.
- `Memory/MemoryLatencyTask.cpp`: `random_device` + `mt19937_64` + `std::shuffle` of the page order.
- `Svg/SvgSuperSampler.cpp`: fixed seed, but `uniform_real_distribution`.
- RSA: OpenSSL's random PKCS#1 padding (encryption) and blinding (decryption), so the output differs on every run.

The timing doesn't depend on the values: matrix multiplication and FFT do the same operations in the same order for
any input, and the inputs never come near denormals. With the same libc++ everywhere the algorithms are the same, so
this doesn't bias scores.

**Decided 2026-11-14:** the Math seeds are fixed (above); the rest is left as is. Cross-platform input/output checksums
are not planned: `scripts/parity/disasm-parity.py` already shows that the platforms run the same code. Memory latency
stays random on purpose: it is lmbench's algorithm (fixed bit-reversed line order within a page, a random page order per
run, one pointer cycle through all pages), and a fixed page order would make every run use the same single sample. If
checksums are wanted later: use in-house distributions for the Math inputs (so a libc++ upgrade can't change them) and
leave RSA out. A different idea that works with random input: let each benchmark verify its own result (decompressed
data equals the input, decrypted equals plaintext, inverse FFT returns the input), which would catch a broken task
scoring high on any platform.

## Found by the parity checks

See [parity-checks.md](parity-checks.md) for the tool and what was already fixed.

- **Linux vs Android, same architecture (2026-11-12):** what remains is the OS layer: mimalloc's OS/prim code
  (`mmap`/`mmap64`, `__errno_location`/`__errno`, NUMA, init, error output), the platform lock and `pthread_atfork`
  calls, libc++'s locale setup (bionic's `ctype` table type) and `call_once`/`__cxa_guard` (glibc vs bionic
  threading). mimalloc's thread id (bionic TLS slot) and lock size per libc are accepted differences, see
  [mimalloc.md](mimalloc.md#code-parity-differences-accepted).
  Third-party code that embeds libc types also differs: on aarch64 `pthread_mutex_t` is 48 bytes on glibc and 40 on bionic, so classes holding a
  `std::mutex` (e.g. LLVM's `TemporaryFiles`) have other field offsets, and bionic's headers change some inlining
  (LLVM's demangler, `LockFileManager`, `Sema`). Compilation on aarch64: 39,738 reachable functions identical, about
  165 different after normalization (~0.4%).
- **The local Linux sysroot is glibc 2.38+.** Binaries built against it need `GLIBC_2.38` (`__isoc23_sscanf` and so
  on from libc++/boost) and won't run on older distributions. The Docker image uses focal (2.31), which doesn't have
  the `__isoc23_*` names.
- **Platform C library calls musl-lite doesn't cover (2026-11-12, every Linux and Windows target):** `report` flags
  calls from the timed code into the platform's `strerror` and (Windows) `rand`/`srand`:
  - Compilation: `strerror`; on Windows `rand` from `createUniquePath` (`GlobalModuleIndex::writeIndex` →
    `LockFileManager`).
  These are reachable, not executed: an `LD_PRELOAD` shim counting calls during full single- and multi-thread
  Compilation runs (x86_64 Linux, 2026-11-14) saw 0 calls to `rand`, `srand` and `strerror` (error paths and LLVM's
  temporary-file code). Windows runs the same code path. **Accepted, not changed** (decided 2026-11-14): musl's
  `strerror` table uses musl's (Linux) errno numbers, which differ from msvcrt's, and musl's `rand` keeps one global
  seed where msvcrt keeps one per thread; neither can affect results.
  Fixed 2026-11-14: ctype and `snprintf`/`vsnprintf`/`sprintf`/`vsprintf` now come from musl-lite on every target
  (they were glibc's `__ctype_*_loc`, msvcrt's `isalpha`/`tolower`/..., and glibc's or mingw-w64's gdtoa-based
  printf, from LLVM, OpenSSL's `CONF_parse_list`, nlohmann's error messages and libc++'s `std::to_string`). See
  [third-party-libraries.md](third-party-libraries.md#musl-lite-srcmusl).
- **`mallinfo` (2026-11-15, every target):** LLVM's `sys::Process::GetMallocUsage()` in the Compilation benchmark calls
  the platform's `mallinfo` (mimalloc doesn't provide it; on Android it is bionic's scudo, which runs `cpuid`).
  Reachable, not executed: only `-track-memory` calls it. Accepted like `strerror`.
- **x86_64 Linux vs Windows ABI (2026-11-15):** structs of floats returned by value come back in SSE registers on
  Linux and in `rax` or memory on Windows, Windows saves `xmm6`-`xmm15` in every function that uses them, and
  `operator new`/`delete` went through the CRT hooks on Windows. Fixed where it cost score: FFT (Windows was 9%
  slower), JSON (12%), SVG (5%), XML (6%), matrix inverse (4%). Left, all 1.5-5% on Windows: JSON (mimalloc's
  `TlsAlloc` slot), C++ compilation, XML, RSA decryption, regex, ray tracing, matrix inverse. Measured under Wine on
  one machine, not yet on real Windows. See
  [parity-checks.md](parity-checks.md#linux-vs-windows-on-one-machine-2026-11-15).
- **Outline atomics in bionic's `libc.a`** are reached from the timed code through mimalloc's locks
  (`pthread_mutex_lock` → `__aarch64_cas4_acq`). The report lists them as violations on Android aarch64.

## Remaining platform differences

- **armv7 float ABI:** `hard` on Linux, `softfp` on Android. See [android.md](android.md#armv7-float-abi).
- **bionic outline atomics on aarch64**, in libc's locking. See
  [android.md](android.md#bionic-outline-atomics-aarch64).
- **boost-regex locale backend** comes from libc++'s per-OS implementation. See
  [third-party-libraries.md](third-party-libraries.md#boost-regex-externboost-regex-standalone).
- **32-bit vs 64-bit**: e.g. OpenSSL's `BN_ULONG`. Unavoidable.
- **`bcmp` on Linux** (2026-11-16): with builtins on, clang turns `memcmp() == 0` into `bcmp` only for some targets.
  It resolves to musl-lite's `bcmp.c`, which calls `memcmp`, so the same code runs plus one call.
- **C++ Compilation Windows/Linux 0.96** (i9-10900K, Wine), with the old and the new source. Not analyzed.

## Not verified

- XML parsing after builtins were enabled (i9-10900K): −4.2% on Windows (Wine), equal on Linux. About half was
  libc++'s hash table calling musl-lite's `memset` for tiny bucket arrays, fixed 2026-11-17 (small-size path in
  `memset.c`); Windows is still −2.1%, Windows/Linux 0.991. See
  [parity-checks.md](parity-checks.md#xml-and-small-memset-calls-2026-11-17).
- The C++ Compilation time budget (about 2.5 s per compile on a Pentium 4 Northwood) is extrapolated from the
  7950X3D, 10900K and older E5200 results; run the new source on an E5200 and a Pentium 4.

- The full Android app build through `dotnet` (the csproj changes) and whether .NET 10 for Android still supports
  API 21.
- The arm64 and armv7 Android builds on real hardware (only emulator translation and `qemu-arm`). The armv7 `app`
  part has only been built.
- Real Android 5 devices with kernels older than 3.10.

## Tooling ideas

Done: the disassembly diff and the binary checks are `scripts/parity/disasm-parity.py`
([parity-checks.md](parity-checks.md)). It could run in CI on every build (`report` exits 1 on violations).

- Per-task input/output checksums, so a platform that does different work shows up immediately (needs deterministic
  input first).
- Instruction counts per task (`valgrind --tool=callgrind`, or `qemu-<arch>` with the insn plugin): builds for the
  same architecture should match exactly.
