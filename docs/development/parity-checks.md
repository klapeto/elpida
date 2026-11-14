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

Accepted: mimalloc's thread id and lock size per libc, see
[mimalloc.md](mimalloc.md#code-parity-differences-accepted). Still open: see [known-issues.md](known-issues.md#found-by-the-parity-checks).
