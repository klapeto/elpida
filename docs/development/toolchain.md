# Toolchain

All targets are built with the llvm-mingw toolchain (clang 21.1.8 with lld, compiler-rt, libunwind and libc++),
through `cmake-modules/llvm-cross.cmake`. Android is covered in [android-toolchain.md](android-toolchain.md).

## Targets

`llvm-cross.cmake` derives the target from `TARGET_PREFIX` (`<arch>-<os>-<env>-`), for example `x86_64-linux-gnu-`,
`i386-w64-mingw32-` or `aarch64-linux-android29-`:

- `w64` gives `CMAKE_SYSTEM_NAME=Windows`.
- `linux` with an `android*` environment gives `CMAKE_SYSTEM_NAME=Linux` plus `ANDROID=ON`. (`CMAKE_SYSTEM_NAME=Android`
  would make CMake look for an NDK.)
- `linux` otherwise gives `Linux`.
- `armv7` and `armv7a` give `CMAKE_SYSTEM_PROCESSOR=arm`.

The compilers are `${TARGET_PREFIX}clang`/`clang++` from `PATH`, and `CMAKE_SYSROOT` is the target's sysroot inside the
toolchain (for example `/mnt/Dev/toolchains/llvm-mingw/x86_64-linux-gnu`).

The toolchain's `.cfg` files select `-stdlib=libc++ -rtlib=compiler-rt -unwindlib=libunwind -fuse-ld=lld` for every
target, so libstdc++ and MSVC STL are never used.

## Linux sysroots and glibc

A Linux binary needs at least the glibc it was linked against: every glibc function has a symbol version (e.g.
`pthread_create@GLIBC_2.34`) and the loader refuses older versions. Newer headers also redirect calls to new functions
(glibc 2.38 turns `strtol`/`sscanf` into `__isoc23_*`, 2.34 moved `pthread_*`/`dl*` into libc with new versions). So
the sysroot's glibc decides the oldest distribution Elpida runs on, not the code.

The Linux sysroots are Ubuntu **focal** (glibc 2.31), in Docker (`DEB_DISTRIBUTION=focal`) and, since 2026-11-13, in
`/mnt/Dev/toolchains/llvm-mingw` (before: noble, glibc 2.38; binaries failed on Ubuntu 20.04/22.04). The fork's
`build-gnu-sysroots.sh` defaults to noble, so always pass `DEB_DISTRIBUTION=focal`. A new sysroot needs everything
installed in it rebuilt, with clean build directories (the `.deb` files keep their old timestamps, so incremental builds
keep objects compiled against the old headers):

```sh
cd /mnt/Dev/sources/llvm-mingw
mv $TOOLCHAIN/<triple> <backup>
DEB_DISTRIBUTION=focal TARGET_TRIPLES=<triple> ./build-gnu-sysroots.sh $TOOLCHAIN
for s in build-compiler-rt.sh build-libcxx.sh build-openmp.sh; do
    CLEAN=1 TARGET_TRIPLES=<triple> ./$s $TOOLCHAIN --additional-cflags-script=$ELPIDA/scripts/docker/parity-flags.sh
done
# then hwloc (build-autotools-based.sh) and the LLVM libraries (build-llvm.sh), see below
```

Check a build (must be 2.31 or lower):

```sh
objdump -T <binary> | grep -o 'GLIBC_[0-9.]*' | sort -uV | tail -1
```

## Runtimes

compiler-rt, libunwind, libc++abi and libc++ get linked into the benchmarks, so they have to be built with the same
parity flags as the benchmarks themselves ([code-parity.md](code-parity.md#compiler-flags)). This repository's
`scripts/docker/parity-flags.sh <triple>` prints them; the llvm-mingw fork has no flags of its own. Its `build-all.sh`,
`build-compiler-rt.sh`, `build-libcxx.sh`, `build-mingw-w64.sh`, `build-mingw-w64-libraries.sh` and `build-openmp.sh`
take `--additional-cflags-script=<path>` and add the script's output for every target. The Dockerfiles
(`docker/build-environment`, `docker/build-environment-android`, `.devcontainer`) copy the script to
`/tmp/parity-flags.sh` and pass it.

To rebuild the runtimes of existing Linux/Windows targets (done for `/mnt/Dev/toolchains/llvm-mingw` on 2026-11-11):

```sh
TOOLCHAIN=/mnt/Dev/toolchains/llvm-mingw
ELPIDA=/mnt/Dev/sources/elpida
cd /mnt/Dev/sources/llvm-mingw
export TARGET_TRIPLES="x86_64-linux-gnu i386-linux-gnu aarch64-linux-gnu armv7-linux-gnueabihf riscv64-linux-gnu \
    x86_64-w64-mingw32 i386-w64-mingw32 aarch64-w64-mingw32"
./build-compiler-rt.sh $TOOLCHAIN --additional-cflags-script=$ELPIDA/scripts/docker/parity-flags.sh
./build-libcxx.sh $TOOLCHAIN --additional-cflags-script=$ELPIDA/scripts/docker/parity-flags.sh
```

The fork's `build-libcxx.sh` passes `-DLIBCXX_HAS_ATOMIC_LIB=OFF` for Linux targets. Without it libc++'s CMake links
GCC's `libatomic` whenever the sysroot contains it (other packages pull in `libatomic1`), although libc++ uses none of
its symbols, and `libc++.so.1` then fails to load on systems without `libatomic1` (e.g. a fresh Kubuntu 24.04).
Check with `readelf -d libc++.so.1 | grep NEEDED`. Elpida itself links libc++, libc++abi and libunwind statically on
Linux (`-static-libstdc++ -static-libgcc`, root `CMakeLists.txt`), like Windows (`-static`) and Android (static-only
runtimes), so its executables need only glibc and the system libraries Qt loads.

**Every** runtime rebuild must pass `--additional-cflags-script`, `build-all.sh` included. A `build-all.sh` without it
(2026-11-11) silently rebuilt every target's runtimes with clang's defaults: SSE4 in Android x86_64 compiler-rt
(`insertps` in `__mulsc3`, reached from the FFT loop), frame pointers and x18 in aarch64 Linux libc++.
`build-all.sh` also runs `build-android-sysroots.sh`, which recreates the Android sysroots and deletes the hwloc and
LLVM libraries installed in them. After a full `build-all.sh`, redo steps 2 and 3 of
[android-toolchain.md](android-toolchain.md). To rebuild only the runtimes, run the individual scripts (with
`build-mingw-w64.sh --with-default-msvcrt=msvcrt --with-default-win32-winnt=0x0501` for Windows XP).

Then check (the flags are also in `llvm-project/runtimes/build-<triple>/CMakeCache.txt`, `CMAKE_CXX_FLAGS_INIT`):

```sh
llvm-nm $TOOLCHAIN/aarch64-linux-gnu/usr/lib/libc++.a | grep -c " U __aarch64_"   # must be 0 (also libc++abi, libunwind)
# libclang_rt.builtins-aarch64.a still shows " U __aarch64_have_lse_atomics": those are the outline-atomics helpers
# themselves (outline_atomic_*.S.o), only linked when called, and the benchmarks are built with -mno-outline-atomics.
```

## LLVM libraries for the Compilation benchmark

The Compilation benchmark links LLVM/Clang **19.1.7** static libraries installed in each target's sysroot (`LLVM_DIR`
is `<sysroot>/usr/lib/cmake/llvm`). They are built by `scripts/docker/build-llvm.sh <triple> <sysroot>`
(`scripts/docker/batch-build.sh` runs it for `$TARGET_TRIPLES`). The script:

- clones the `llvmorg-19.1.7` tag (pinned: the benchmark measures this code, so every target must use the same LLVM);
- applies `scripts/docker/llvm.parity.patch`, which disables the Lexer's SSE2/SSE4.2/AltiVec paths;
- adds `-DLLVM_XXH_USE_NEON=0` and the parity flags (the same per-architecture flags as the root `CMakeLists.txt`);
- builds only the X86 backend (the benchmark always compiles for `x86_64-none-none`), with assertions off.

The libraries of the Linux and Windows targets in `/mnt/Dev/toolchains/llvm-mingw` were rebuilt with the current
script on 2026-11-10.

## Checks

```sh
# No outline atomics (aarch64)
llvm-nm <binary> | grep -c " U __aarch64_"
# musl and mimalloc are in the benchmark (defined, not imported). Not in stdlib-benchmarks, which uses the platform's.
llvm-nm <benchmark> | grep -wE "T (malloc|memcpy|memchr|strtod)"
# The StdLib math loops really call the library (the platform's; see code-parity.md): count the calls in the
# task's lambda (expect 10000).
# clang loads the function address once and calls through a register (callq *%r14, blr x20), so count calls:
llvm-objdump -d --no-show-raw-insn <stdlib-benchmarks> | \
    awk '/<_ZZN6Elpida10Benchmarks6StdLib9FloorTask9DoRunImplEvENK.*>:$/{p=1;next} p&&/^$/{exit} p' | grep -cE "call|blr|bl\s"
```
