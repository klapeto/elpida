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

this_dir="$(readlink -f "$(dirname "$0")")"
. ${this_dir}/build-base.sh

CFLAGS="--sysroot=$SYSROOT"

HOST_TRIPLE=$(gcc -dumpmachine)

if [ "$TARGET_TRIPLE" != "$HOST_TRIPLE" ]; then
  ../configure \
    -static -no-shared -release \
    -submodules qtcharts,qtsvg,qtbase \
    -nomake examples -nomake tests -nomake benchmarks -nomake manual-tests -nomake minimal-static-tests \
    -prefix "$INSTALL_PREFIX" \
    -qt-host-path "$SYSROOT_BASE/$HOST_TRIPLE/usr" -- \
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
    -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY
else
  ../configure \
    -static -no-shared -release \
    -submodules qtcharts,qtsvg,qtbase \
    -nomake examples -nomake tests -nomake benchmarks -nomake manual-tests -nomake minimal-static-tests \
    -prefix "$INSTALL_PREFIX" -- \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_ASM_COMPILER="$CC" \
    -DCMAKE_C_FLAGS_INIT="$CFLAGS" \
    -DCMAKE_CXX_FLAGS_INIT="$CFLAGS" \
    -DCMAKE_LINKER_TYPE=LLD \
    -DCMAKE_SYSROOT="$SYSROOT" \
    -DCMAKE_FIND_ROOT_PATH="$SYSROOT" \
    -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
    -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
    -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
    -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY
fi

cmake --build . --parallel
cmake --install .
rm -rf ./*
