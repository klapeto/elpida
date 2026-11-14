# Building for Android

Step by step: the Android targets of the llvm-mingw toolchain, the libraries they need, and the app's native code.
For the design, see [android.md](android.md).

## Requirements

- The llvm-mingw toolchain (clang 21.1.8), built from the fork that contains `build-android-sysroots.sh`. Below it is at `/mnt/Dev/toolchains/llvm-mingw` (`$TOOLCHAIN`) and the fork is at
  `/mnt/Dev/sources/llvm-mingw`, with `llvm-project` checked out at `llvmorg-21.1.8`.
- Android NDK r29 (`29.0.14206865`). Only its bionic sysroot is used (headers, crt objects, `libc`/`libm`/`liblog`/...);
  its clang, libc++ and compiler-rt are not used. `build-android-sysroots.sh` downloads it from Google into the fork's
  `cache/` and extracts only the sysroot (about 300 MB), so no NDK has to be installed. `ANDROID_NDK_VERSION` selects
  another release; set `ANDROID_NDK` to use a local NDK instead.
- CMake, Ninja, and an hwloc 2.4 source tree with a generated `configure`.

```sh
TOOLCHAIN=/mnt/Dev/toolchains/llvm-mingw
ELPIDA=/mnt/Dev/sources/elpida
export PATH="$TOOLCHAIN/bin:$PATH"
```

Every target exists at two API levels: **29** for the benchmarks and **21** (the minSdk) for `libElpida.so` and the
tools.

| Triple (`<triple>`) | Android ABI |
|---|---|
| `x86_64-linux-android` | `x86_64` |
| `aarch64-linux-android` | `arm64-v8a` |
| `armv7a-linux-androideabi` | `armeabi-v7a` |

## Docker (CI)

`docker/build-environment-android/Dockerfile` does steps 1–3 automatically. CI builds it with
`--build-arg IMAGE=<build-environment-base image>`.

- **Stage `toolchain`** starts from the base image, which already has the host clang, the llvm-mingw fork in
  `/tmp/llvm-mingw` (`unified-toolchain` branch), hwloc 2.4.0 and `llvm-project` at `llvmorg-19.1.7`. It does step 1 for
  both API levels (`build-android-sysroots.sh` downloads NDK `ANDROID_NDK_VERSION`) (with
  `scripts/docker/parity-flags.sh` copied to `/tmp`), hwloc for every sysroot and `build-llvm.sh` for every
  `<triple>29`.
- **The final stage** is the Android SDK + .NET image. It gets the toolchain at `/opt/toolchains/llvm`
  (`ELPIDA_TOOLCHAIN`, which the csproj uses), CMake 4.2.3 and Ninja. It has no NDK: .NET for Android doesn't need one
  (its Mono AOT for Release brings its own tools), so `AndroidNdkDirectory` is not set anywhere. Verified 2026-11-11 with
  a Release `dotnet publish` against an SDK without `ndk/`. Setting `AndroidNdkDirectory` to an invalid path does fail
  (XA5104 in the AOT step), so leave it unset rather than pointing it somewhere empty.

Build arguments: `ANDROID_NDK_VERSION`, `ANDROID_TRIPLES`, `ANDROID_BENCHMARKS_API` (29) and `ANDROID_APP_API` (21).
The fork's `build-android-sysroots.sh` and `--additional-cflags-script` support must be on its `unified-toolchain`
branch on GitHub,
because the base image clones it from there. The image builds LLVM three times (once per ABI), so it takes several
hours.

## 1. Toolchain targets and runtimes

```sh
cd /mnt/Dev/sources/llvm-mingw
export TARGET_TRIPLES="x86_64-linux-android29 aarch64-linux-android29 armv7a-linux-androideabi29 \
    x86_64-linux-android21 aarch64-linux-android21 armv7a-linux-androideabi21"
./build-android-sysroots.sh $TOOLCHAIN   # downloads the NDK; or ANDROID_NDK=/path/to/ndk
./install-wrappers.sh $TOOLCHAIN
./build-compiler-rt.sh $TOOLCHAIN --additional-cflags-script=$ELPIDA/scripts/docker/parity-flags.sh
./build-libcxx.sh $TOOLCHAIN --additional-cflags-script=$ELPIDA/scripts/docker/parity-flags.sh
```

(`build-all.sh` with the same `TARGET_TRIPLES` and `--additional-cflags-script` does the same, plus
LLVM itself.)

For every triple and API level this creates:

| What | Where |
|---|---|
| Compiler wrappers, named like the NDK's | `$TOOLCHAIN/bin/<triple><api>-clang`, `-clang++`, `-ar`, ... |
| Sysroot: bionic from the NDK, without its C++ runtime | `$TOOLCHAIN/<triple><api>/` |
| compiler-rt builtins | `$TOOLCHAIN/lib/clang/21/lib/<arch>-unknown-linux-android<api>/libclang_rt.builtins.a` |
| libunwind, libc++abi, libc++ (static) | `$TOOLCHAIN/<triple><api>/usr/{include,lib}` |

`build-android-sysroots.sh` only creates the sysroot; the runtimes come from `build-compiler-rt.sh` and
`build-libcxx.sh`, with the parity flags. An existing sysroot is kept, because it also holds
the libraries installed in the next steps; `CLEAN=1` recreates it.

The API level is part of the tool name because `clang-target-wrapper.sh` passes its own name as `--target`. A
`-target` in the `.cfg` would be overridden.

## 2. LLVM libraries (API 29 only)

The Compilation benchmark links LLVM/Clang 19.1.7. Only the `benchmarks` part needs them.

```sh
mkdir -p ~/build/llvm-android && cd ~/build/llvm-android   # build-llvm.sh clones ./llvm-project here
for t in x86_64-linux-android29 aarch64-linux-android29 armv7a-linux-androideabi29; do
    $ELPIDA/scripts/docker/build-llvm.sh $t $TOOLCHAIN/$t
done
```

`build-llvm.sh` checks out the `llvmorg-19.1.7` tag, applies `llvm.parity.patch` (once) and installs into
`<sysroot>/usr`. Each target takes a while: about 30–60 minutes on a fast machine.

## 3. hwloc (every sysroot)

`elpida-core` links hwloc, so both API levels need it. Use the same options as `scripts/docker/batch-build.sh`:

```sh
HWLOC_OPTIONS="--enable-plugins=no --disable-readme --disable-cairo --disable-libxml2 --disable-io --disable-pci \
  --disable-opencl --disable-cuda --disable-nvml --disable-rsmi --disable-levelzero --disable-gl --disable-libudev"
cd <directory containing the hwloc source as ./hwloc>
for api in 29 21; do
    for t in x86_64-linux-android aarch64-linux-android armv7a-linux-androideabi; do
        $ELPIDA/scripts/docker/build-autotools-based.sh $t$api $TOOLCHAIN/$t$api ./hwloc/ "$HWLOC_OPTIONS"
    done
done
```

## 4. The app

`Elpida.Mobile.csproj` (target `BuildNativeLibrary`) builds the native code before the Android manifest is generated.
For every ABI it runs two CMake builds of `Platforms/Android/Native`:

| Build | `ELPIDA_ANDROID_PART` | `TARGET_PREFIX` | Output |
|---|---|---|---|
| `Platforms/Android/libs/<abi>/benchmarks` | `benchmarks` | `<triple>29-` | `lib*-benchmarks.so` (static executables), all but StdLib |
| `Platforms/Android/libs/<abi>/app` | `app` | `<triple>21-` | `libElpida.so`, `libelpida-info-dumper.so`, `libstdlib-benchmarks.so` (dynamic, device libc) |

MSBuild properties:

| Property | Default | Meaning |
|---|---|---|
| `ElpidaToolchainDirectory` | `$(ELPIDA_TOOLCHAIN)`, then `/mnt/Dev/toolchains/llvm-mingw` | The llvm-mingw toolchain |
| `ElpidaAndroidBenchmarksApi` | `29` | API level of the benchmarks (must be >= 29 for ELF TLS) |
| `ElpidaAndroidAppApi` | `21` | API level of the app part; must match the minSdk (`SupportedOSPlatformVersion`) |

To build one part by hand (here the x86_64 benchmarks):

```sh
cmake -S $ELPIDA/apps/Elpida.Mobile/Elpida.Mobile/Platforms/Android/Native -B build-android-x86_64-benchmarks -G Ninja \
    -DCMAKE_BUILD_TYPE=Release -DELPIDA_ENABLE_CLI=OFF -DELPIDA_ENABLE_QT=OFF -DELPIDA_ENABLE_APPLICATION_LIBRARIES=ON \
    -DELPIDA_ANDROID_PART=benchmarks \
    -DCMAKE_TOOLCHAIN_FILE=$ELPIDA/cmake-modules/llvm-cross.cmake \
    -DTARGET_PREFIX=x86_64-linux-android29- -DCMAKE_SYSROOT=$TOOLCHAIN/x86_64-linux-android29
cmake --build build-android-x86_64-benchmarks
```

For the `app` part, use `-DELPIDA_ANDROID_PART=app`, `TARGET_PREFIX=<triple>21-` and the `<triple>21` sysroot. The app
part sets `ELPIDA_ENABLE_BENCHMARKS=OFF` and `ELPIDA_ENABLE_STDLIB_BENCHMARKS=ON`, so it doesn't need LLVM.

## Checking the binaries

```sh
llvm-readelf -lW libweb-benchmarks.so | grep -E "interpreter|TLS"   # benchmarks: no interpreter (static), a TLS segment
llvm-nm libweb-benchmarks.so | grep -E " __mi_theap_default$"       # benchmarks: mimalloc uses __thread
llvm-nm libweb-benchmarks.so | grep -c __emutls_get_address         # benchmarks: 0
llvm-readelf -SW libElpida.so | grep " .hash "                      # app part: SysV hash, needed before API 23
```

## Testing without the app

Push the executables to a device or emulator and run them the way `libElpida.so` does:

```sh
adb shell mkdir -p /data/local/tmp/elpida
adb push libs/x86_64/benchmarks/lib*-benchmarks.so libs/x86_64/app/libelpida-info-dumper.so /data/local/tmp/elpida/
adb shell 'chmod 755 /data/local/tmp/elpida/*; cd /data/local/tmp/elpida && \
    ./libelpida-info-dumper.so --benchmarks-directory=/data/local/tmp/elpida --benchmarks-suffix=-benchmarks.so'
adb shell 'cd /data/local/tmp/elpida && ./libweb-benchmarks.so --index=1 --affinity=0 --format=json --config=128'
```

- Recent x86_64 Google emulator images (API 30+) run arm64 binaries through their ARM translation, so the arm64 build
  can be smoke-tested there. Timings under translation are meaningless.
- The static armv7 benchmarks run on a Linux host under `qemu-arm-static`, inside a PID namespace (32-bit bionic
  needs pids <= 65535): `unshare -Urpf --mount-proc qemu-arm-static ./libstdlib-benchmarks.so --dump-info`.
- Android 5 has no `timeout` command.
