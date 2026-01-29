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

folders=("Qt" "CLI")

for fn in "${folders[@]}"; do
  mkdir cmake-build-debug/apps/$fn/Benchmarks
  ln -s "../InfoDumper/elpida-info-dumper" "cmake-build-debug/apps/$fn/elpida-info-dumper"
  ln -s "../../../src/Benchmarks/Compression/elpida-compression-benchmarks" "cmake-build-debug/apps/$fn/Benchmarks/elpida-compression-benchmarks"
  ln -s "../../../src/Benchmarks/Encryption/elpida-encryption-benchmarks" "cmake-build-debug/apps/$fn/Benchmarks/elpida-encryption-benchmarks"
  ln -s "../../../src/Benchmarks/Image/elpida-image-benchmarks" "cmake-build-debug/apps/$fn/Benchmarks/elpida-image-benchmarks"
  ln -s "../../../src/Benchmarks/Math/elpida-math-benchmarks" "cmake-build-debug/apps/$fn/Benchmarks/elpida-math-benchmarks"
  ln -s "../../../src/Benchmarks/Memory/elpida-memory-benchmarks" "cmake-build-debug/apps/$fn/Benchmarks/elpida-memory-benchmarks"
  ln -s "../../../src/Benchmarks/StdLib/elpida-stdlib-benchmarks" "cmake-build-debug/apps/$fn/Benchmarks/elpida-stdlib-benchmarks"
  ln -s "../../../src/Benchmarks/Web/elpida-web-benchmarks" "cmake-build-debug/apps/$fn/Benchmarks/elpida-web-benchmarks"
  ln -s "../../../src/Benchmarks/Compilation/elpida-compilation-benchmarks" "cmake-build-debug/apps/$fn/Benchmarks/elpida-compilation-benchmarks"
done


