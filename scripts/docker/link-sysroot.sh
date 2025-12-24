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
  echo "usage: $0 source-sysroot output-dir [triple]"
  exit 1;
fi

if [ -z "$2" ]; then
  echo "usage: $0 source-sysroot output-dir [triple]"
  exit 1;
fi

if [ -n "$3" ]; then
  TRIPLE="$3"
fi

INPUT_SYSROOT=$(readlink -f "$1")
OUTPUT_DIR=$(readlink -f "$2")

mkdir -p "$OUTPUT_DIR/usr"

if [ -d "$INPUT_SYSROOT/usr/bin" ]; then
  ln -s "$INPUT_SYSROOT/usr/bin" "$OUTPUT_DIR/usr/bin"
elif [ -d "$INPUT_SYSROOT/bin" ]; then
  ln -s "$INPUT_SYSROOT/bin" "$OUTPUT_DIR/usr/bin"
fi

if [ -z "$TRIPLE" ]; then
  if [ -d "$INPUT_SYSROOT/usr/lib" ]; then
    ln -s "$INPUT_SYSROOT/usr/lib" "$OUTPUT_DIR/usr/lib"
  elif [ -d "$INPUT_SYSROOT/lib" ]; then
    ln -s "$INPUT_SYSROOT/lib" "$OUTPUT_DIR/usr/lib"
  fi
else
  ln -s "$INPUT_SYSROOT/usr/lib/$TRIPLE" "$OUTPUT_DIR/usr/lib"
  ln -s "$INPUT_SYSROOT/usr/lib/$TRIPLE" "$INPUT_SYSROOT/usr/lib/$TRIPLE/$TRIPLE"
fi

if [ -d "$INPUT_SYSROOT/usr/include" ]; then
  ln -s "$INPUT_SYSROOT/usr/include" "$OUTPUT_DIR/usr/include"
elif [ -d "$INPUT_SYSROOT/include" ]; then
  ln -s "$INPUT_SYSROOT/include" "$OUTPUT_DIR/usr/include"
fi

if [ -z "$TRIPLE" ]; then
  if [ -d "$OUTPUT_DIR/usr/bin" ]; then
    ln -s "usr/bin" "$OUTPUT_DIR/bin"
  fi
  ln -s "usr/lib" "$OUTPUT_DIR/lib"
  ln -s "usr/include" "$OUTPUT_DIR/include"
fi
