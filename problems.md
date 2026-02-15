- General:
    - Qt6 needs host `libxkbcommon-x11-dev`
    - Had to move host `/usr/include` to `/usr/_include` to avoid conflicts with Sysroot headers.
    - Had not have gtk3 installed to avoid having to search for wayland scanner.
- Ubuntu:bionic
    - LLVM 18.1.0: failed to link due to missing `dladdr` function
    - LLVM 19.1.0: compiled but failed to launch due to a missing library (fs something). Update: it was this "fix" I
      tried to cross-compile for the host architecture:
      `ENV LD_LIBRARY_PATH="/opt/sysroots/$HOST_TRIPLE/lib:/opt/sysroots/$HOST_TRIPLE/lib/$HOST_TRIPLE"`
    - LLVM 20.1.0: failed to compile due to
      `use of deleted function 'std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&)`.
    - LLVM 21.1.0: failed to compile due to
      `use of deleted function 'std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&)`.
    - Added `ppa:ubuntu-toolchain-r/test` to install GCC 13
    - LLVM 21.1.0: compiled with GCC 11.
      - Could not cross-compile as host target. Opted to compile natively.
      - Qt6 does not enable xcb problem. `libxkbcommon-x11-dev` was a version lower than 0.9.0 that qt needed
      - Managed to build Qt6 using generated toolchain files. Still LLVM fails in `i386` to link due to missing `dladdr` function
    - Launching the latest CMake from repositories could not find libssl 1.1.1. Had to download from binaries instead.
    - on i386/aarch64/etc the symbolic links to libdl.so.2, etc. were rooted. this caused the "missing `dladdr` function" because it was looking for them in the root library.
- Ubuntu:focal
 - i386 does not have `libxcb-cursor-dev` package