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

if [ $# -ne 3 ]; then
    echo "Usage $0 target-triple sysroot-target-dir source-dir [additional flags]"
    exit 1;
fi


directory="$3"
additionalFlags="$4"

mkdir -p "$directory/build"
cd "$directory"
NOCONFIGURE=1 ./autogen.sh
cd "build"

../configure \
    CC="$CC" \
    CXX="$CXX" \
    CFLAGS="$CFLAGS" \
    CXXFLAGS="$CXXFLAGS" \
    --host="$TARGET_TRIPLE" \
    --with-sysroot="$SYSROOT" \
    --prefix="$INSTALL_PREFIX" \
    --enable-static \
    "$additionalFlags"

make -j$(nproc)
make -j$(nproc) install
rm -rf ./*
cd ../..
