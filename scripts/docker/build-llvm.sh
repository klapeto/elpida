#!/bin/bash
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

set -e

this_dir="$(readlink -f "$(dirname "$0")")"
. ${this_dir}/build-base.sh

if [ ! -d ./llvm-project ]; then
    git clone --depth=1 -b release/19.x https://github.com/llvm/llvm-project.git
fi

mkdir -p ./llvm-project/llvm/build
cd ./llvm-project/llvm/build

# we need -fno-builtin because on arm64 windows the compilation benchmark crashes (wtf?)
CFLAGS="-fno-builtin $CFLAGS"

cmake -S .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX" \
  -DCMAKE_C_COMPILER="$CC" \
  -DCMAKE_CXX_COMPILER="$CXX" \
  -DCMAKE_ASM_COMPILER="$CC" \
  -DCMAKE_C_FLAGS_INIT="$CFLAGS" \
  -DCMAKE_CXX_FLAGS_INIT="$CFLAGS" \
  -DCMAKE_LINKER_TYPE=LLD \
  -DCMAKE_SYSTEM_NAME="$TARGET_OS" \
  -DCMAKE_SYSTEM_PROCESSOR="$TARGET_ARCH" \
  -DCMAKE_SYSROOT="$SYSROOT" \
  -DCMAKE_FIND_ROOT_PATH="$SYSROOT" \
  -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
  -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
  -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
  -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY \
  -DLLVM_ENABLE_PROJECTS=clang \
  -DENABLE_STATIC=ON \
  -DCMAKE_SKIP_INSTALL_RPATH=ON \
  -DBUILD_SHARED_LIBS=OFF \
  -DLLVM_STATIC_LINK_CXX_STDLIB=ON \
  -DLIBCLANG_BUILD_STATIC=ON \
  -DLLVM_TARGETS_TO_BUILD=X86 \
  -DLLVM_ENABLE_ZSTD=OFF \
  -DLLVM_BUILD_DOCS=OFF \
  -DLLVM_BUILD_EXAMPLES=OFF \
  -DLLVM_BUILD_RUNTIME=OFF \
  -DLLVM_BUILD_RUNTIMES=OFF \
  -DLLVM_BUILD_TESTS=OFF \
  -DLLVM_BUILD_TOOLS=OFF \
  -DLLVM_BUILD_UTILS=OFF \
  -DLLVM_ENABLE_BINDINGS=OFF \
  -DLLVM_ENABLE_DOXYGEN=OFF \
  -DLLVM_ENABLE_OCAMLDOC=OFF \
  -DLLVM_ENABLE_SPHINX=OFF \
  -DLLVM_INCLUDE_BENCHMARKS=OFF \
  -DLLVM_INCLUDE_EXAMPLES=OFF \
  -DLLVM_INCLUDE_TESTS=OFF \
  -DLLVM_INCLUDE_DOCS=OFF \
  -DLLVM_INCLUDE_GO_TESTS=OFF \
  -DLLVM_INCLUDE_RUNTIMES=OFF \
  -DLLVM_INCLUDE_TOOLS=ON \
  -DLLVM_INCLUDE_UTILS=OFF \
  -DLLVM_BUILD_BENCHMARKS=OFF \
  -DLLVM_BUILD_INSTRUMENTED_COVERAGE=OFF \
  -DLLVM_BUILD_LLVM_DYLIB=OFF \
  -DLLVM_ENABLE_DIA_SDK=OFF \
  -DLLVM_ENABLE_DOXYGEN_QT_HELP=OFF \
  -DLLVM_ENABLE_HTTPLIB=OFF \
  -DLLVM_ENABLE_IDE=OFF \
  -DLLVM_ENABLE_LIBPFM=OFF \
  -DLLVM_INDIVIDUAL_TEST_COVERAGE=OFF \
  -DLLVM_LINK_LLVM_DYLIB=OFF

make -j$(nproc) llvm-tblgen llvm-config clang-tblgen install
rm -rf ./*
cd ../../..