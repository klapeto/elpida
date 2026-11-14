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

: ${TARGET_TRIPLES:="x86_64-linux-android29 aarch64-linux-android29 armv7a-linux-androideabi29 x86_64-linux-android21 aarch64-linux-android21 armv7a-linux-androideabi21 aarch64-w64-mingw32 x86_64-w64-mingw32 i386-w64-mingw32 x86_64-linux-gnu i386-linux-gnu armv7-linux-gnueabihf aarch64-linux-gnu riscv64-linux-gnu"}

for target_triple in $TARGET_TRIPLES; do
    ./build-llvm.sh $target_triple /mnt/Dev/toolchains/llvm-mingw/$target_triple/
    ./build-autotools-based.sh $target_triple /mnt/Dev/toolchains/llvm-mingw/$target_triple/ ./hwloc/ "--enable-plugins=no --disable-readme --disable-cairo --disable-libxml2 --disable-io --disable-pci --disable-opencl --disable-cuda --disable-nvml --disable-rsmi --disable-levelzero --disable-gl --disable-libudev"
done


