# Android

How the Android app runs the benchmarks so they execute the same code as on the other platforms, while still running
on Android 5.0 and newer. Building it is described in [android-toolchain.md](android-toolchain.md).

## Overview

The Android app runs the **same benchmark executables as the desktop apps**, built with the same llvm-mingw clang,
libc++, musl-lite and mimalloc. The native code is split in two parts:

| Part | Built for | Contents | Linking |
|---|---|---|---|
| `benchmarks` | API 29 | the `*-benchmarks` executables | **static** |
| `app` | API 21 (the minSdk) | `libElpida.so` (P/Invoked by the .NET app) and `elpida-info-dumper` | dynamic |

Only the benchmarks have to run identical code, so only they get the strict treatment. The `app` part is ordinary
Android code that the device's own loader has to accept.

The benchmarks are statically linked so they can be built for API 29 and still run on Android 5.0 (API 21). That gives
them native ELF TLS and mimalloc's `__thread` model everywhere (see [TLS](#tls)), and it removes the device's `libc.so`
from the benchmarks: their bionic is the NDK's `libc.a`, the same on every device.

The exception is `stdlib-benchmarks` (`libstdlib-benchmarks.so`), which measures the device's own C library
([code-parity.md](code-parity.md#the-stdlib-benchmarks)). It is built in the `app` part: an API 21 PIE executable,
dynamically linked against `libc.so`/`libm.so` (`/system/bin/linker64`), without musl-lite and mimalloc. It doesn't
need ELF TLS (no mimalloc), so API 21 is fine. All 8 tasks verified on the API 21 x86_64 emulator.

Sources: `apps/Elpida.Mobile/Elpida.Mobile/Platforms/Android/Native/` (`CMakeLists.txt`, `Elpida.cpp`) and the
`BuildNativeLibrary` target of `Elpida.Mobile.csproj`.

## Run model

1. The .NET app calls `Load()` in `libElpida.so` with `ApplicationInfo.NativeLibraryDir`.
2. `Load()` runs `libelpida-info-dumper.so --benchmarks-directory=<dir> --benchmarks-suffix=-benchmarks.so`, like the
   desktop `InfoGetter`. The dumper reports the system information, the timing overheads and the benchmark list (it
   runs every `lib*-benchmarks.so` with `--dump-info`).
3. `ModelBuilderJson` parses that, and `OffProcessBenchmarkExecutionService` runs each benchmark as its own process,
   exactly like the CLI and Qt apps.

Earlier the benchmarks were `MODULE` libraries `dlopen`ed into the app process (`InProcessBenchmarkExecutionService`,
`DynamicLoadedBenchmark`). That ran them inside the .NET runtime's process, with bionic's allocator (a library cannot
replace `malloc` there) and with the overheads measured in-process. Both are gone.

### Packaging

- Android only lets apps execute files from the native library directory, and it only extracts files named
  `lib*.so` there. So every executable is named `lib<name>.so` (`elpida_android_executable()` in the native
  `CMakeLists.txt`).
- `AndroidManifest.xml` sets `android:extractNativeLibs="true"`, so the libraries are extracted at install time
  instead of being loaded from the APK.
- The csproj adds them as `AndroidNativeLibrary` items with an explicit `Abi`, because the ABI cannot be inferred from
  the `libs/<abi>/app|benchmarks/` paths.
- Everything is linked with `-z max-page-size=16384`, for devices with 16 KB pages.

## TLS

Android before 10 (API 29) has no ELF TLS in its dynamic loader. For lower API levels clang compiles every
`__thread`/`thread_local` as **emulated TLS**: each access calls `__emutls_get_address()`.

Measured on the emulator (x86_64, KVM on the same host CPU as the Linux numbers):

| Operation | Linux | Android, native TLS | Android, emulated TLS |
|---|---|---|---|
| thread-local get+set | 1.79 ns | 1.78 ns | 7.3 ns |
| `pthread_getspecific`+`setspecific` | 4.4 ns | 4.1 ns | (same) |

What that means for the benchmarks:

- **mimalloc** keeps its per-thread heap in TLS. It defaults to `MI_TLS_MODEL_PTHREADS` (`pthread_getspecific`) on
  Android and to `__thread` on Linux. malloc+free(48): 2.85 ns on Linux, **4.55 ns** on Android with the pthread
  model, 2.80 ns with `__thread`. With emulated TLS, mimalloc cannot use `__thread` at all:
  `__emutls_get_address()` allocates the TLS block with `malloc`, recursing into mimalloc until the stack overflows
  (SIGSEGV, reproduced).
- **LLVM** (Compilation benchmark): about 45K TLS accesses per compile, about 0.02% even when emulated.
- **libc++abi** `eh_globals`: only used when exceptions are thrown; none are thrown during the benchmarks.
- Elpida's own code has no `thread_local`.

Measured effect of the pthread model on whole benchmarks (3 rounds each, Android emulator): XML parsing −4.7% and JSON
parsing −4.8%, with no overlap between the two variants' results. C++ compilation, RSA, PNG encoding, the SVG
benchmarks and Boost regex were within noise (±2–3%).

So `src/Mimalloc/CMakeLists.txt` defines `MI_TLS_MODEL_LOCAL=1` for Android API >= 29. The static API 29 benchmarks
get it on every Android version, because their TLS is set up by the `libc.a` inside them, not by the device's
loader. The `app` part (API 21) never links mimalloc.

Raising minSdk to 29 would have been simpler, but would drop Android 5–9 devices
([principle 2](README.md#principles)).

## ABIs and supported devices

| ABI | Toolchain triples | Tested |
|---|---|---|
| `x86_64` | `x86_64-linux-android29` / `21` | All benchmarks on emulators: Android 5.0 (API 21, kernel 3.10) and Android 17 (API 37) |
| `arm64-v8a` | `aarch64-linux-android29` / `21` | One benchmark per group, through the emulator's ARM translation (API 37) |
| `armeabi-v7a` | `armv7a-linux-androideabi29` / `21` | One benchmark per group, static executables under `qemu-arm` (the `app` part only builds) |

The native code should run on Android 5.0 and newer phones and tablets with any of these ABIs. 32-bit x86 Android
devices are not built (they are rare). Notes:

- Not verified: that .NET for Android / MAUI (the app's `net10.0-android` framework) still supports API 21, and the
  full app build through `dotnet`.

- Real-device kernels older than 3.10 have not been tested with the static benchmarks (they contain current bionic).
- 32-bit bionic aborts if a process id is above 65535 (`pthread_mutex_t` stores it in 16 bits). bionic relies on
  Android keeping `pid_max` at or below that where 32-bit code runs; not verified on a real device. On a desktop host, run armv7 binaries in a PID namespace: `unshare -Urpf --mount-proc
  qemu-arm-static ./libstdlib-benchmarks.so ...`.
- Memory: `memset`/`memchr` use 256 MB buffers by default, and the Compilation benchmark executable is about 110 MB.

### armv7 float ABI

Linux (`armv7-linux-gnueabihf`) and Android (`armv7a-linux-androideabi`) use the same ISA features (ARMv7-A,
VFPv3-D32, NEON), but Linux uses the `hard` float calling convention and Android `softfp`. On Android, float/double
arguments and return values of non-inlined calls go through core registers (extra `vmov`s), for example musl's math
functions called from the Math tasks. This is fixed by the platform ABI: building the static benchmarks hard-float
would silently break their calls into bionic's softfp `libc.a`.

### bionic outline atomics (aarch64)

The NDK's prebuilt `libc.a` uses outline atomics (61 references: `pthread_mutex`, rwlock, condition variables,
`pthread_create`/`exit`, `__cxa_guard`), so the static aarch64 benchmarks contain LSE/LL-SC runtime dispatch inside
libc's locking. It is not in the benchmarks' hot loops (thread start/stop and synchronization only), and desktop
aarch64 Linux has the same in the distribution's glibc. Removing it would mean building bionic from source.

## Other details

- **x86 baseline:** the Android triple makes clang enable SSE4.2/POPCNT/CX16 (x86_64) and SSSE3 (i686) by default.
  The parity flags pin the same baseline as on desktop ([code-parity.md](code-parity.md#cpu-baselines)).
- **`ANDROID` macro:** the NDK toolchain file defines `-DANDROID`, and httplib depends on it (without it, it uses
  `getifaddrs()`, which only exists from API 24). The root `CMakeLists.txt` defines it for Android builds. No
  benchmark code checks it.
- **Hash style:** loaders before API 23 need a SysV `.hash` table. clang adds it automatically when targeting API 21,
  so the `app` part has it. A dynamically linked binary built for API 29 fails on Android 5 with
  `empty/missing DT_HASH`. The static benchmarks don't use the loader.
- **PIC:** only the libraries `libElpida.so` links are built position-independent. The benchmarks are PIE, like on
  desktop Linux. (The NDK toolchain file used to enable PIC globally.)
