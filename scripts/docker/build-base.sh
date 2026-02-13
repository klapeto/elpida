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

# exit if any command fails
set -e

if [ ! $# -ge 2 ]; then
    echo "Usage $0 target-triple sysroot-target-dir"
    exit 1;
fi

TARGET_TRIPLE="$1"
SYSROOT="$2"
if [ -z "${TARGET_TRIPLE}" ]; then
    echo "Provided empty target triple: $TARGET_TRIPLE"
    exit 1;
fi

if [ -z "$SYSROOT" ]; then
    echo "Provided sysroot target dir: $SYSROOT"
    exit 1;
fi

TARGET_ARCH=$(expr match "$TARGET_TRIPLE" '\(.*\)-.*-.*')
TARGET_OS=$(expr match "$TARGET_TRIPLE" '.*-\(.*\)-.*')
TARGET_PREFIX="$TARGET_TRIPLE-"
INSTALL_PREFIX="$SYSROOT/usr"

if [ `command -v gcc` ]; then
    HOST_TRIPLE=$(gcc -dumpmachine)
else
    CLANG_HOST_TRIPLE=$(clang -dumpmachine)
    CLANG_HOST_ARCH=$(expr match "$CLANG_HOST_TRIPLE" '\(.*\)-.*-.*-.*')
    CLANG_HOST_OS=$(expr match "$CLANG_HOST_TRIPLE" '.*-.*-\(.*\)-.*')
    CLANG_HOST_ENV=$(expr match "$CLANG_HOST_TRIPLE" '.*-.*-.*-\(.*\)')
    HOST_TRIPLE="$CLANG_HOST_ARCH-$CLANG_HOST_OS-$CLANG_HOST_ENV"
fi

if [ "$TARGET_OS" = "linux" ]; then
    TARGET_OS=Linux
elif [ "$TARGET_OS" = "w64" ]; then
    TARGET_OS=Windows
fi

case $TARGET_ARCH in
armv7-*)
    TARGET_PROCESSOR=arm
    ;;
*)
    TARGET_PROCESSOR=$TARGET_ARCH
    ;;
esac

CC="${TARGET_PREFIX}clang"
CXX="${TARGET_PREFIX}clang++"
RANLIB="${TARGET_PREFIX}ranlib"
AR="${TARGET_PREFIX}ar"
STRIP="${TARGET_PREFIX}strip"
CFLAGS="--sysroot=$SYSROOT"
CXXFLAGS="--sysroot=$SYSROOT"

echo "Compiling for '$TARGET_TRIPLE' using '${CC}' and will install to '${INSTALL_PREFIX}'"
