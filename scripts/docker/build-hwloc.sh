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

if [ ! -d ./hwloc ]; then
    git clone --depth=1 -b hwloc-2.4.0 https://github.com/open-mpi/hwloc.git
    cd hwloc
    ./autogen.sh
    cd ..
fi

mkdir -p hwloc/build
cd hwloc/build

../configure \
    CC=$CC \
    CXX=$CXX \
    CFLAGS="$CFLAGS" \
    CXXFLAGS="$CXXFLAGS" \
    --host="$TARGET_TRIPLE" \
    --with-sysroot="$SYSROOT" \
    --prefix="$INSTALL_PREFIX" \
    --enable-static \
    --disable-shared \
    --enable-plugins=no \
    --disable-readme \
    --disable-cairo \
    --disable-libxml2 \
    --disable-io \
    --disable-pci \
    --disable-opencl \
    --disable-cuda \
    --disable-nvml \
    --disable-rsmi \
    --disable-levelzero \
    --disable-gl \
    --disable-libudev

make -j$(nproc) install
rm -rf ./*
cd ../..
