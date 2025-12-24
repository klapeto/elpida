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

if [ -z "${TARGET_TRIPLE}" ]; then
    TARGET_TRIPLE=$(gcc -dumpmachine)
fi

TARGET_ARCH=$(expr match "$TARGET_TRIPLE" '\(.*\)-.*-.*')

TARGET_OS=$(expr match "$TARGET_TRIPLE" '.*-\(.*\)-.*')

if [ "$TARGET_OS" = "linux" ]; then
    TARGET_OS=Linux
elif [ "$TARGET_OS" = "w64" ]; then
    TARGET_OS=Windows
fi

if [ -z "${TARGET_PREFIX}" ]; then
    TARGET_PREFIX=$TARGET_TRIPLE-
fi

if [ -z "${SYSROOT_BASE}" ]; then
    SYSROOT_BASE=/opt/sysroots
    #SYSROOT_BASE=/mnt/Dev/sysroots/
fi

SYSROOT=$SYSROOT_BASE/$TARGET_TRIPLE

if [ -z "${INSTALL_PREFIX}" ]; then
    if [ -z "${INSTALL_BASE}" ]; then
        INSTALL_PREFIX="$SYSROOT/usr"
    else
        INSTALL_PREFIX="$INSTALL_BASE/$TARGET_TRIPLE/usr"
    fi
fi

CC="${TARGET_PREFIX}clang"
CXX="${TARGET_PREFIX}clang++"
RANLIB="${TARGET_PREFIX}ranlib"
AR="${TARGET_PREFIX}ar"
STRIP="${TARGET_PREFIX}strip"
TARGET_TRIPLE="$TARGET_TRIPLE"
TARGET_PREFIX="$TARGET_PREFIX"
SYSROOT_BASE="$SYSROOT_BASE"
SYSROOT="$SYSROOT"
INSTALL_PREFIX="$INSTALL_PREFIX"
TARGET_ARCH="$TARGET_ARCH"
TARGET_OS="$TARGET_OS"

echo "Compiling for '$TARGET_TRIPLE' using '${CC}' and will install to '${INSTALL_PREFIX}'"
