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

## Non-deterministic input

Several tasks generate their input with `std::random_device` seeds, `std::normal_distribution`,
`std::uniform_int_distribution` or `std::shuffle`:

- `Math/MatrixMultiplication32Task.cpp`, `Math/DynamicMatrixMultiplicationTask.cpp`,
  `Math/CalculateFFTInPlaceTask.cpp`: `random_device` + `normal_distribution`.
- `Memory/MemoryLatencyTask.cpp`: `random_device` + `mt19937_64` + `std::shuffle`.
- `Svg/SvgSuperSampler.cpp`: fixed seed, but `uniform_real_distribution`.

The timing hardly depends on the values (no denormals, the memory latency chain is always one full cycle), and with
the same libc++ version everywhere the algorithms are the same, so this doesn't bias scores. But the input differs on
every run, which prevents reproducing a run or verifying parity with input/output checksums. Fix: fixed seeds and
in-house distributions (raw `mt19937` output, Box–Muller, Fisher–Yates).

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
- **Outline atomics in bionic's `libc.a`** are reached from the timed code through mimalloc's locks
  (`pthread_mutex_lock` → `__aarch64_cas4_acq`). The report lists them as violations on Android aarch64.

## Remaining platform differences

- **armv7 float ABI:** `hard` on Linux, `softfp` on Android. See [android.md](android.md#armv7-float-abi).
- **bionic outline atomics on aarch64**, in libc's locking. See
  [android.md](android.md#bionic-outline-atomics-aarch64).
- **boost-regex locale backend** comes from libc++'s per-OS implementation. See
  [third-party-libraries.md](third-party-libraries.md#boost-regex-externboost-regex-standalone).
- **32-bit vs 64-bit**: e.g. OpenSSL's `BN_ULONG`. Unavoidable.

## Not verified

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
