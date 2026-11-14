#!/bin/sh
#
#  Copyright (c) 2025  Ioannis Panagiotopoulos
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
# Benchmark code parity (docs/development/code-parity.md): the compiler flags for code that ends up in the benchmarks
# but is built outside this CMake project, i.e. the toolchain runtimes (compiler-rt, libunwind, libc++abi, libc++) and
# the LLVM libraries of the Compilation benchmark (build-llvm.sh). They must match ELPIDA_PARITY_FLAGS in the root
# CMakeLists.txt.
#
# Usage: parity-flags.sh <target-triple>   (prints the flags)
#
# For the llvm-mingw build scripts (build-all.sh, build-compiler-rt.sh, build-libcxx.sh, ...):
#   --additional-cflags-script=/path/to/parity-flags.sh

# -fomit-frame-pointer: clang keeps frame pointers by default on Android and aarch64 Linux only
flags="-fno-math-errno -ffp-contract=off -fomit-frame-pointer"

case ${1%%-*} in
aarch64|arm64)
    # outline atomics choose LSE or LL/SC at runtime; x18 is reserved on Android and Windows only;
    # the Cortex-A53 erratum 835769 workaround is on by default for Android only
    flags="$flags -mno-outline-atomics -ffixed-x18 -mfix-cortex-a53-835769"
    ;;
x86_64)
    # fixed baseline: the Android triple enables SSE4.2/POPCNT/CX16 by default
    flags="$flags -march=x86-64 -mtune=generic -mno-sse3 -mno-ssse3 -mno-sse4.1 -mno-sse4.2 -mno-popcnt -mno-cx16"
    ;;
i386|i586|i686)
    flags="$flags -march=pentium4 -mtune=generic -mno-sse3 -mno-ssse3"
    ;;
*)
    # armv7, riscv64, ...: no arch-specific flags, the common ones above are enough
    ;;
esac
echo "$flags"
