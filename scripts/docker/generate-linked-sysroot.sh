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

if [ -z "$1" ]; then
  echo "usage: $0 source-sysroot triple output-dir"
fi

if [ -z "$2" ]; then
  echo "usage: $0 source-sysroot triple output-dir"
fi

if [ -z "$3" ]; then
  echo "usage: $0 source-sysroot triple output-dir"
fi

INPUT_SYSROOT=$(readlink -f "$1")
TRIPLE=$2
OUTPUT_DIR=$(readlink -f "$3")

mkdir -p "$OUTPUT_DIR/usr"
ln -s "$INPUT_SYSROOT/usr/lib/$TRIPLE" "$OUTPUT_DIR/usr/lib"
ln -s "$INPUT_SYSROOT/usr/include" "$OUTPUT_DIR/usr/include"
ln -s "usr/lib" "$OUTPUT_DIR/lib"
ln -s "usr/include" "$OUTPUT_DIR/include"
