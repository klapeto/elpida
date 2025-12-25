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
  echo "usage: $0 arch sourceUrl triple"
  exit 1;
fi

if [ -z "$2" ]; then
  echo "usage: $0 arch sourceUrl triple"
  exit 1;
fi

if [ -z "$3" ]; then
  echo "usage: $0 arch sourceUrl triple"
  exit 1;
fi

if [ -f "./$3/usr/lib/$3/libc.a" ]; then
  echo "Sysroot seems to already exist. To force, remove the following directory '$PWD/$3'"
  exit 0;
fi

if [ ! -f ./SysrootGenerator ]; then
    wget -O SysrootGenerator https://github.com/klapeto/SysrootGenerator/releases/download/v1.0.1/SysrootGenerator-linux-x86-64
fi

chmod +x ./SysrootGenerator;

./SysrootGenerator --arch="$1" --distribution=noble --sources="$2|main,universe" --packages=`paste -sd, packages` --path="$3";

