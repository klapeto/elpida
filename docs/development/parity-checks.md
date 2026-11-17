# Parity checks

`scripts/parity/disasm-parity.py` checks the machine code that the timed part of every task runs. It starts from each
`Task::DoRunImpl()`, follows direct calls, tail calls and function addresses taken in the code, and collects every
function the timed code can reach. See [code-parity.md](code-parity.md) for the policy it checks.

Needs Python 3 and the toolchain's `llvm-objdump`, `llvm-nm`, `llvm-cxxfilt` and `llvm-readelf`. They are taken from
`--toolchain`, then `$ELPIDA_TOOLCHAIN/bin`, then `PATH`. Binaries need their symbol table. Release builds keep it on
every platform (`.exe` included); packaging may strip it, so check the build tree's binaries.

## Commands

```sh
export ELPIDA_TOOLCHAIN=/mnt/Dev/toolchains/llvm-mingw
P=scripts/parity/disasm-parity.py

# Per task: functions/instructions reached, imports, and violations. Exit code 1 on any violation.
$P report build/apps/Benchmarks/math-benchmarks
$P report -v build/apps/Benchmarks/math-benchmarks            # also every reached function and import

# Two builds of the same architecture, task by task. Exit code 1 if any task differs.
$P compare build-linux/apps/Benchmarks/math-benchmarks build-android/libmath-benchmarks.so
$P compare A B --show-diff 30                                 # with a unified diff per changed function

# Normalized disassembly per function, for diff -r or meld
$P dump build-linux/apps/Benchmarks/math-benchmarks /tmp/linux
$P dump build-android/libmath-benchmarks.so /tmp/android && diff -r /tmp/linux /tmp/android
```

The StdLib tasks measure the platform's C library on purpose ([code-parity.md](code-parity.md#the-stdlib-benchmarks)),
so they are skipped: `--exclude` defaults to `^Elpida::Benchmarks::StdLib::`. `--exclude ''` checks them anyway, e.g.
`--exclude '' report -v stdlib-benchmarks` to confirm that they call glibc/msvcrt/bionic.

Global options (before the command): `--roots REGEX` picks other timed functions (default `::DoRunImpl\(\)$`, e.g.
`--roots 'FFTInPlaceTask::DoRunImpl\(\)$'` for one task), `--mnemonics` compares only instruction mnemonics (for
different ABIs, such as Linux vs Windows), `--stop REGEX` changes which functions are not followed, and
`--max-functions N` limits the search (needed for the Compilation benchmark, which reaches most of clang).

## What `report` flags

- **CPU detection:** `cpuid`, `xgetbv`, `rdrand`/`rdseed`, aarch64 `mrs` of `ID_AA64*`/`MIDR_EL1`.
- **Runtime dispatch helpers:** calls to `__aarch64_cas*`/`swp*`/`ld*` (outline atomics), `__cpu_indicator_init`,
  `__cpu_model`, `__cpu_features*`, `init_have_lse_atomics`, `IsProcessorFeaturePresent`, `android_getCpu*`,
  `OPENSSL_cpuid_setup`, `OPENSSL_ia32cap*`.
- **IFUNCs:** `IRELATIVE` relocations anywhere in an ELF binary.
- **Platform libc doing the work:** calls into a shared library or DLL for a function musl-lite or mimalloc provides on
  every platform (string, stdlib, ctype, stdio formatting, math, the allocator). On Windows the CRT's `malloc`/`free`
  are expected, because mimalloc hooks them at runtime. Other imports (memory mapping, threads, time, I/O) are
  listed with `-v` and are not violations.

Error and exception paths are listed but not followed (`--stop`; the unwinder, `__cxa_throw`, `abort`, assertion
handlers, `std::__throw_*`). They don't run in the timed loop and pull in large parts of the runtime.

## How `compare` normalizes

Two links of the same code differ in addresses, so before comparing:

- Branch targets in the same function become offsets (`<.+0x30>`). Calls keep the demangled target name, so a call
  that resolves to another function, or to an import, is a difference.
- PC-relative displacements (`X(%rip)`) and aarch64 `adrp` page offsets (`#lo12`) are removed. A symbol named in the
  instruction's comment is kept; constants without a symbol become `<?>`.
- On 32-bit targets, absolute addresses inside the image become `ADDR`.
- x86 static TLS offsets (`%fs:-0x38`, `%gs:-0x8`) become `%fs:-TLS`: a variable's place in the TLS block is chosen
  at link time. Positive offsets (stack guard `%fs:0x28`, TCB/TEB fields) are ABI and kept. On aarch64, the `add`s
  that follow `mrs xN, TPIDR_EL0` become `#TLS` (bionic reserves a larger area before the TLS block than glibc).
- aarch64 `adrp` pages become `<page>` (objdump names them after whatever symbol starts the page), and lld's
  layout-dependent relaxation `nop`+`adr` is written back as `adrp`+`add`.
- riscv64 `auipc` immediates become `<hi>` and the low part in the instruction that uses the register becomes `lo`.
- ARM literal pools (`.word` data inside functions: PC-relative offsets, addresses, float constants) become
  `.word <literal>`.
- lld's range extension thunks (`__ARMV7PILongThunk_foo`, `__AArch64ADRPThunk_foo`, ...) exist only when the target is
  out of range, which depends on the layout: a call to the thunk is written as a call to `foo`, and `foo` is followed.
- Trailing padding (`nop`, `int3`, `udf`) is dropped.

objdump leaves some references unresolved; the tool resolves them so the calls are followed and imports are seen:

- aarch64 `adrp`+`ldr` GOT loads (calls with `-fno-plt`) and `adrp`+`add`, from the GOT slot (its `RELATIVE`
  relocation or its contents in a static binary).
- riscv64 `auipc`+`addi`/`ld`/`jalr` pairs, and PLT entries (objdump names them `<write+0x39d9f0>`): each PLT entry's
  `auipc`+`ld` gives its GOT slot, whose `JUMP_SLOT` relocation names the import. Calls the linker relaxed to `jal`
  are resolved the same way.
- i386 Windows import thunks (`jmpl *0x51e928`), from the `__imp_` symbols.
- ARM: `--disassemble-symbols` stops at a function's first literal pool, so the whole image is disassembled once and
  split at the function symbols (armv7 Compilation: about 40 s and 1.7 GB per binary).

## Limitations

- Calls through vtables or function pointers loaded from data (OpenSSL's `EVP_*` method tables, `std::function`,
  virtual calls) cannot be followed. They are counted as "indirect calls". Use `--roots` to add the targets as roots.
- armv7 function addresses loaded from literal pools or built with `movw`/`movt` are not resolved (direct `bl` calls
  are), so code reached only through them is missed.
- riscv64: lld relaxes `auipc`+`jalr` to `jal` when the target is close, so two links differ where the layout does.
  There is no other riscv64 build to compare with yet; `report` is what matters there.
- armv7 Android calls `__aeabi_memcpy*`/`__aeabi_memmove*`/`__aeabi_memset*` (compiler-rt forwarders to musl-lite's
  functions) where Linux calls `memcpy`/`memmove`/`memset`: the calls show up as differences.
- Suspicious names (`OPENSSL_cpuid_setup`, ...) are not flagged when the function only returns (OpenSSL is built
  `no-asm`). Code that really probes the CPU is still caught by its instructions.
- `--mnemonics` across ABIs (Linux vs Windows) still shows differences from the calling convention and the
  `long double` format. Use it to spot a different algorithm or instruction mix, not as a pass/fail check.

- The default `--max-functions 5000` truncates the Compilation and Encryption benchmarks; use
  `--max-functions 200000` for them, otherwise functions past the limit show up as "0 instructions" differences.
- Local (`static`) functions with the same name in several objects (OpenSSL's `static inline` helpers such as
  `ossl_size_add`, `CRYPTO_UP_REF`) are matched by name, so the tool may compare two different copies. Two symbols at
  one address (e.g. `ASTDeclReader::VisitBuiltinTemplateDecl`/`VisitVarTemplateDecl`) can appear under either name.
  Both show up when comparing links that keep different sets of functions (`--gc-sections` on/off); they are not code
  changes.

## Findings (2026-11-09, x86_64 and aarch64; Linux, Android, Windows)

Fixed:

- Android kept frame pointers (clang's default there and on aarch64 Linux), so every function differed: added
  `-fomit-frame-pointer` to the parity flags.
- `x18` was allocatable on aarch64 Linux only: added `-ffixed-x18` for aarch64.
- Linux took compiler builtins (`__mulsc3` in the FFT loop) from the sysroot's GCC `libgcc.a`: removed `-lgcc`.
- `strtoul` calls went to glibc's `__isoc23_strtoul` (glibc >= 2.38 headers) and `strtod` calls on Windows to mingw's
  gdtoa `__strtod`: musl-lite now defines those names. (Found in the StdLib tasks, which now measure the platform's
  library on purpose; the fix matters for the other benchmarks.) See
  [third-party-libraries.md](third-party-libraries.md#musl-lite-srcmusl).
- `bcmp` came from glibc (an IFUNC): added to musl-lite.
- mimalloc ran `cpuid` (write-only): skipped outside MSVC.
- (2026-11-12) clang enables the Cortex-A53 erratum workarounds (`nop`s before multiply-accumulates, `adrp` patching)
  only for Android: now enabled for every aarch64 target.
- (2026-11-12) A `build-all.sh` run without `--additional-cflags-script` rebuilt every runtime without the parity
  flags (SSE4 in Android x86_64 `__mulsc3`, frame pointers and x18 in aarch64 libc++). See
  [toolchain.md](toolchain.md#runtimes).
- musl-lite's `strtod` computed in `long double` (128-bit in software on aarch64 and Android x86_64): now `double`
  everywhere. See [third-party-libraries.md](third-party-libraries.md#musl-lite-srcmusl).

## Findings (2026-11-12, armv7, i386, riscv64; Linux, Android, Windows)

- Fixed: i386 Windows ran mingw-w64's `strtoll`/`strtoull` (with msvcrt's `isspace`/`isdigit` per character) in the
  JSON number parsing, because libc++'s `_strtoi64` import pulled in mingw-w64's objects that define them too. See
  [third-party-libraries.md](third-party-libraries.md#musl-lite-srcmusl). Found once the tool saw i386 imports.
- armv7 Linux vs Android: zlib and the other integer code are identical. The differences are the float ABI (`hard`
  vs `softfp`: `vmov`s around calls with float arguments, in all float code), the OS layer, and mimalloc's lock size
  (24-byte `pthread_mutex_t` in glibc vs 4 in bionic), all known. Compilation: 31,901 of 38,465 functions identical.
- i386 and riscv64 Linux report the same violations as x86_64 Linux, i386 Windows the msvcrt equivalents. See
  [known-issues.md](known-issues.md#found-by-the-parity-checks).

## Findings (2026-11-14, ctype and snprintf in musl-lite)

- x86_64 Linux and x86_64/i386 Windows: the ctype and `snprintf` violations are gone; what's left is `strerror` and
  (Windows) `rand`/`srand`. The Windows JSON task now reaches musl's `printf_core` instead of `__mingw_pformat`.
- The tool counted every `__imp_` symbol as an import. musl-lite now defines `__imp_isalpha` and the like itself, so
  only `__imp_` slots inside the PE's IAT (data directory 12; lld merges `.idata` into `.rdata`) are imports; the
  others are followed to the function they point to, also for i386's `calll *0x...`.

## Findings (2026-11-15, CI binaries, all targets)

The checks ran on the binaries of CI pipeline 3068900155 (the Docker toolchain), all 8 desktop targets and the 3
Android ABIs.

- `report`: only the known items (`strerror`/`rand`/`srand`, bionic's outline atomics on Android aarch64) and one
  new reachable-but-never-run path: LLVM's `sys::Process::GetMallocUsage()` calls the platform's `mallinfo` (glibc,
  msvcrt, bionic's scudo, which runs `cpuid`). It's only called with LLVM's `-track-memory`, which is off
  (`llvm/lib/Support/Timer.cpp`). See [known-issues.md](known-issues.md#found-by-the-parity-checks).
- Linux vs Android, same architecture: the known OS layer, libc type sizes and, on x86_64, `long double` (musl's
  `pop_arg` for `%Lf`, never used).
- The tool crashed on COFF symbol tables with non-UTF-8 names (some read-only data in the CI Windows binaries); it now
  replaces those bytes.
- `compare` didn't compare functions whose names contain `std::size_t` and friends between ELF and PE: 64-bit Windows
  is LLP64, so they demangle as `unsigned long long` (`Matrix<float, 4ull, 4ull>`) there and `unsigned long` on Linux,
  and were listed as "only in A/B". When one side is a PE, both are now written with `long`, in the names and in the
  call targets. Before this, matrix inverse and Base64 looked identical between Linux and Windows.

## Linux vs Windows on one machine (2026-11-15)

The same code is compiled with the same compiler and flags for both, but the two x86_64 ABIs differ (System V on
Linux, Microsoft x64 on Windows), and clang applies the ABI when it writes the IR, before optimization. Inlining
removes a call, not the shape it gave the code, so the optimizer starts from different IR and can decide differently
(vectorize or not, inline or not). The differences that mattered:

| | Linux (System V) | Windows (Microsoft x64) |
|---|---|---|
| Struct of two floats returned (`std::complex<float>`, `SvgPoint`) | `<2 x float>` in `xmm0` | 64-bit integer in `rax` |
| 16-byte struct returned (`SvgColor`) | `xmm0` + `xmm1` | memory (hidden pointer) |
| SSE registers a function must preserve | none | `xmm6`-`xmm15` |
| `long` | 64-bit | 32-bit |
| `operator new`/`delete` reach mimalloc | directly (`malloc` is mimalloc's) | through the CRT's `malloc`/`free`, patched at runtime (`static.c`) |

aarch64 Linux and Windows use the same rules for these (HFA in `s0`-`s3`, same callee-saved registers), so this is an
x86_64 (and i386, not measured) topic.

### How it was measured

i9-10900K, Ubuntu 26.04, `performance` governor, one core (`--affinity=2`), single-threaded, the benchmark
executables run directly with the full run's configuration (`apps/Application/FullBenchmarkInstances`), Linux and
Windows interleaved, median of 7 rounds. The Windows builds ran under Wine 10 (GE-Proton's `files/bin/wine64`): the
timed code makes no Windows API calls, so it runs natively. `scripts/parity/linux-vs-windows.sh` does this. The
spread is mostly under 2%; Base64 decode and matrix multiplication vary up to 10% between runs on Linux (code and
data placement), so differences of 1-2% are noise. The FFT and SVG results were the same on a Ryzen 7950X3D.
Multi-thread results under Wine say nothing about Windows (Linux schedules the threads), and nothing here replaces a
run on real Windows.

Profiling under Wine: `perf record` sees the samples but not the PE's symbols (`[JIT]`); map the `perf script -F ip`
addresses inside `ImageBase` (0x140000000) to `llvm-nm -n` of the `.exe`.

### Results (Windows / Linux score)

| Benchmark | Before | After | Cause | Change |
|---|---|---|---|---|
| FFT | 0.909 | 1.050 | `complex<float>` returned in `rax`: the butterfly was packed on Linux, scalar on Windows | `FFTCalculator` does the arithmetic on the real and imaginary parts |
| JSON parsing | 0.876 | 0.950 | `new`/`delete` went through the CRT hook and `mi_cfree`'s ownership check | `mimalloc-new-delete.h` in the entry point |
| XML parsing | 0.942 | 0.973 | same | same |
| SVG rasterization | 0.952 | 0.987 | `xmm6`+ saves once per sub-sample, `SvgColor` through memory | point test and color wrappers inlined |
| Matrix inverse 4x4 | 0.962 | 0.982 | `CalculateMinor` saved `xmm6`-`xmm15` on every call | inverse chain inlined |
| C++ compilation | 0.969 | 0.974 | allocation (partly fixed), the rest not analyzed | |
| RSA decryption | 0.977 | 0.977 | not analyzed | |
| RSA encryption | 0.985 | 0.984 | | |
| Regex | 1.009 | 0.981 | `new`/`delete`: Linux gained 3.8%, Windows 0.8% | |
| Ray tracing | 1.004 | 0.984 | same code before and after; data placement (allocation order changed) | |
| Base64 decode | 1.081 | 1.021 | code placement: identical code, moves with `-falign-functions=64`, 10% spread | |
| NBody, Base64 encode, Zlib, AES, matrix multiplication | 0.993-1.007 | 0.993-1.007 | | |

Score changes on the same CPU (both platforms change, so the base scores must be recalculated):

| Benchmark | Linux | Windows |
|---|---|---|
| FFT | 2.16x | 2.44x |
| Matrix inverse 4x4 | +24% | +27% |
| SVG rasterization | +3.8% | +7.7% |
| XML parsing | +5.3% | +8.7% |
| Regex | +3.8% | +0.8% |
| JSON parsing | +2.9% | +9.9% |

### The causes and the changes

- **FFT:** libc++'s `operator*`/`operator-` for `complex<float>` return `<2 x float>` on Linux and `i64` on Windows.
  The integer packing (`movd`/`shlq`/`orq`) stays after inlining and the SLP vectorizer judged packing not worth it.
  `FFTCalculator` now does the complex arithmetic on the real and imaginary parts, in `std::complex`'s order, so the
  results are bit-identical (only `__mulsc3`'s NaN recovery is gone, which finite input never reaches). Both
  targets vectorize the loop and differ only in the prologue. The 2.16x on Linux comes from the loop no longer
  checking every product for NaN and no longer reloading the vector's data pointer.
- **SVG rasterization:** `SvgPolygon::IsPointInsideEvenOdd` (62% of the cycles) ran the same loop on both but saved
  `xmm6`-`xmm9` on every call, once per sub-sample. The per-sample color wrappers (`SvgCalculatedPaint::CalculateColor`,
  the gradients' `CalculateColor`) return `SvgColor` through memory on Windows and took about twice the time there.
  They are in the headers now and `[[gnu::always_inline]]`, so they inline into `SvgSuperSampler` on every target.
  The attribute is needed: clang's inlining cost counts the Windows memory copies, and without it the Windows build
  kept `SvgCalculatedPaint::CalculateColor` as a call. The rendered PNGs are byte-identical (old/new, Linux/Windows).
  Left: the call to `CalculatePad`, a real function with its loop, whose `SvgColor` still goes through memory.
- **Matrix inverse:** `Matrix<float, 4, 4>::CalculateMinor` uses all 16 SSE registers; called once per inverse, it
  saved and restored ten of them each time on Windows (78 stack accesses against 59). The inverse chain in
  `Matrix.hpp` is `[[gnu::always_inline]]` now, so it inlines into the timed loop.
- **Allocation:** on Windows libc++'s `operator new`/`delete` call the CRT's `malloc`/`free`, which mimalloc patches
  at runtime (`static.c`): `free` went through `__h_free` and `mi_cfree`, which first looks up whether the pointer is
  mimalloc's, and took twice the time of Linux's `mi_free`. The parity entry point now includes
  `mimalloc-new-delete.h`, so `operator delete` is a jump to `mi_free` on every platform. `new` and `delete` always pair
  up, so `mi_free` never gets a CRT pointer; C code (`malloc`/`free`) still goes through the hooks.
- **Not changed, mimalloc's TLS:** on Windows mimalloc finds the thread's heap through a `TlsAlloc` slot
  (`MI_TLS_MODEL_WIN32`: load the slot number, read the TEB, check for the expansion slots), on Linux with one load at
  a fixed offset. JSON still spends about 1.5x the time in `mi_new` on Windows. `MI_TLS_MODEL_LOCAL` (the exe's static
  TLS: `_tls_index`, `%gs:0x58`, the slot) was tried and made no consistent difference (JSON -1.5%, XML and regex
  +0.5-0.8%), so mimalloc is left as is.
- **Not changed, `long`:** zlib's `uLong` arithmetic (`adler32_z`, `crc32_z`) and boost's reference counts use 32-bit
  instructions on Windows; on x86_64 they cost the same.
- **Code placement:** the same functions start at different offsets in the two builds, and the 10900K (Comet Lake,
  with the JCC erratum microcode) is sensitive to where loops and jumps fall. `-falign-functions=64
  -mbranches-within-32B-boundaries` moved Base64 decode from 1.09 to 1.03 and changed nothing else; not adopted.

Accepted: mimalloc's thread id and lock size per libc, see
[mimalloc.md](mimalloc.md#code-parity-differences-accepted). Still open: see [known-issues.md](known-issues.md#found-by-the-parity-checks).

### On real Windows (2026-11-15)

The same before/after builds on the same 10900K's Windows partition (Windows 10 Pro 20H2, VBS off), single-thread
the same way as on Linux (one core, 7 rounds, before and after interleaved), plus two full runs with `elpida-cli`
each. Linux is the native run on the same machine. Before = 6125c403, after = 2ee5c5e8.

Full run scores (two runs each):

| | Single-thread | Multi-thread | Total |
|---|---|---|---|
| Windows before | 3.14 / 3.16 | 15.9 / 16.1 | 19.4 / 19.7 |
| Windows after | 3.52 / 3.55 | 17.6 / 17.4 | 21.6 / 21.4 |
| Linux before | 3.22 / 3.21 | 16.1 / 15.9 | 19.7 / 19.6 |
| Linux after | 3.52 / 3.50 | 17.1 / 17.2 | 21.1 / 21.2 |

Single-thread Windows / Linux on real Windows, against what Wine predicted:

| Benchmark | Before | After | Under Wine (after) |
|---|---|---|---|
| FFT | 0.906 | 1.049 | 1.050 |
| JSON parsing | 0.890 | 0.958 | 0.950 |
| XML parsing | 0.951 | 0.988 | 0.973 |
| SVG rasterization | 0.949 | 0.984 | 0.987 |
| Matrix inverse 4x4 | 0.973 | 1.008 | 0.982 |
| C++ compilation | 0.962 | 0.971 | 0.974 |
| Regex | 1.005 | 0.975 | 0.981 |
| Ray tracing | 1.007 | 0.980 | 0.984 |
| Base64 decode | 1.035 | 1.124 | 1.021 |
| the others | 0.986-1.003 | 0.985-1.000 | |

Wine predicts real Windows to about 2% for single-thread code, so the Wine method above holds. The single-thread
gap is closed (0.976 before, 1.005 after). Multi-thread differs per benchmark from 0.965 (C++ compilation) to 1.145
(matrix inverse; matrix multiplication and RSA decryption +8-9%) with Windows 2% ahead in total; the single-thread
code runs at the same speed, so that is the OS (thread scheduling and placement). Results:
`results-windows-20261115-095508` (`out/test-2026-11-15/elpida-parity-windows.zip`, `run.ps1`) and
`out/test-2026-11-15/results-linux-20261115-024054`.

### Run-to-run spread

(max - min) / median of the rounds above, one core of the 10900K. Windows without its round 7: something else ran
during it and matrix multiplication, NBody, RSA, regex, SVG and XML dropped to 20-70% in that round only.

| Benchmark | Linux before | Linux after | Windows before | Windows after |
|---|---|---|---|---|
| AES Decryption | 0.1% | 0.1% | 0.5% | 0.5% |
| AES Encryption | 0.1% | 0.2% | 0.6% | 0.4% |
| Base64 Decode | 11.1% | 10.1% | 7.7% | 0.4% |
| Base64 Encode | 1.6% | 0.4% | 4.2% | 0.5% |
| C++ Compilation | 3.4% | 1.4% | 1.1% | 2.6% |
| FFT | 1.2% | 0.7% | 0.8% | 0.4% |
| Json Parsing | 5.1% | 1.2% | 0.4% | 0.7% |
| Matrix inverse 4x4 | 2.5% | 3.0% | 4.0% | 1.4% |
| Matrix mult 32x32 | 2.8% | 1.0% | 2.3% | 1.1% |
| NBody | 0.1% | 0.5% | 0.3% | 1.0% |
| RSA Decryption | 0.8% | 1.7% | 0.2% | 0.3% |
| RSA Encryption | 0.1% | 1.3% | 0.3% | 0.2% |
| Ray Tracing | 0.2% | 0.3% | 0.5% | 0.3% |
| Regex | 1.4% | 1.7% | 0.8% | 0.6% |
| SVG Rasterization | 0.3% | 0.4% | 0.8% | 0.4% |
| XML Parsing | 0.9% | 1.2% | 0.9% | 0.6% |
| Zlib compression | 0.8% | 0.9% | 0.5% | 1.7% |
| Zlib decompression | 0.4% | 0.5% | 2.6% | 0.4% |

Within one binary the code offsets are fixed (ASLR moves whole pages), so this spread comes from what changes
between processes. The inputs are fixed (seeds), and the fast and slow rounds of before and after do not coincide
(the state of the machine is not it). It is the address layout: ASLR, and mimalloc's random start of its aligned
allocations. Both must be off for the runs to repeat, and fixing them is not a fix (one lucky or unlucky layout per
binary). Part of the spread measured on the 10900K was the `powersave` governor: with `performance` the other web benchmarks are
0.4-2.8%.

Base64 decode was the outlier (11-12%): `base64_chars.find()`'s `memchr` loop exits after a data-dependent number of
steps, and how well the branch predictor learns that depended on the layout. It also gave the same 10900K a 1.109
Windows/Linux ratio and reversed the order of two CPUs. The decoder now uses a lookup table, and both encode and decode are written like production Base64 code. Measurements on six
machines, the mechanism, Ice Lake against Comet Lake and the test scripts: [base64-decode-spread.md](base64-decode-spread.md).
Every other benchmark, checked the same way under `perf stat`: [run-to-run-spread.md](run-to-run-spread.md).

### Zlib compression and code placement

After / before was 0.977 on Linux and 0.979 on Windows. The timed loop does not allocate (`deflate` into a stack
buffer, `MemoryStream::Write` after `Reset` keeps its capacity) and zlib's code is the same in both builds, only
0x2a0 bytes later in after (32 bytes modulo 64; the entry point grew). Built with `-falign-functions=64`, the hot
functions (`longest_match`, `deflate_slow`, `fill_window`) start on 64 bytes in both, and the difference is gone
(15 rounds on the 10900K: before 1.000, after 0.969, before aligned 0.997, after aligned 0.998). On the 7950X3D the
same two unaligned builds went the other way (after faster). A change anywhere in front of a benchmark's code can
move its score by a few percent on some CPUs.

