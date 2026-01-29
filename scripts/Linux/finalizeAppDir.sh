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

if [ "$#" -lt 1 ]; then
    echo "Usage: $0 <appdir> <relative-executable> <name>"
    exit 1
fi

INSTALL_DIR=$1
EXECUTABLE=$2
NAME=$3

if [[ -z $EXECUTABLE ]]; then
  EXECUTABLE="bin/elpida-qt"
fi

if [[ -z $NAME ]]; then
  NAME="Elpida.tar.xz"
fi

CURDIR=$PWD
this_script_dir="$(readlink -f "$(dirname "$0")")"
cp "$this_script_dir/elpida-entry-point.sh" "$INSTALL_DIR/elpida"
{
  printf "\n"
  if [ -f "$INSTALL_DIR/ld.env.sh" ]; then
        tail -n +2 "$INSTALL_DIR/ld.env.sh"
  fi
  echo "exec \"\$this_dir/$EXECUTABLE\" \"\$@\""
} >> "$INSTALL_DIR/elpida"
chmod +x "$INSTALL_DIR/elpida"
cd "$INSTALL_DIR"
tar --exclude="ld.env.sh" -cJf "$CURDIR/$NAME" .