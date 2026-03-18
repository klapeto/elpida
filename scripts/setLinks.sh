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

folders=("Qt" "CLI")

if [ ! $# -eq 1 ]; then
    echo "Usage $0 root-dir"
    exit 1;
fi

function link() {
  if [ ! -f "$2" ]; then
    ln -s "$1" "$2"
  fi
}

for fn in "${folders[@]}"; do
  if [ ! -d $directory ]; then
    mkdir -p "$1/apps/$fn/Benchmarks"
  fi
  link "../InfoDumper/elpida-info-dumper" "$1/apps/$fn/elpida-info-dumper"
  link "../../../src/Benchmarks/Compression/elpida-compression-benchmarks" "$1/apps/$fn/Benchmarks/elpida-compression-benchmarks"
  link "../../../src/Benchmarks/Encryption/elpida-encryption-benchmarks" "$1/apps/$fn/Benchmarks/elpida-encryption-benchmarks"
  link "../../../src/Benchmarks/Image/elpida-image-benchmarks" "$1/apps/$fn/Benchmarks/elpida-image-benchmarks"
  link "../../../src/Benchmarks/Math/elpida-math-benchmarks" "$1/apps/$fn/Benchmarks/elpida-math-benchmarks"
  link "../../../src/Benchmarks/Memory/elpida-memory-benchmarks" "$1/apps/$fn/Benchmarks/elpida-memory-benchmarks"
  link "../../../src/Benchmarks/StdLib/elpida-stdlib-benchmarks" "$1/apps/$fn/Benchmarks/elpida-stdlib-benchmarks"
  link "../../../src/Benchmarks/Web/elpida-web-benchmarks" "$1/apps/$fn/Benchmarks/elpida-web-benchmarks"
  link "../../../src/Benchmarks/Compilation/elpida-compilation-benchmarks" "$1/apps/$fn/Benchmarks/elpida-compilation-benchmarks"
done


