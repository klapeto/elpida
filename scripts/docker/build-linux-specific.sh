#!/bin/sh
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

# exit if any command fails
set -e

this_dir="$(readlink -f "$(dirname "$0")")"
. ${this_dir}/build-base.sh

case "$TARGET_TRIPLE" in
*-linux-gnu*)
    ;;
*-w64-mingw32)
    exit 0
    ;;
esac

PKG_CONFIG_PATH=/opt/sysroots/$TARGET_TRIPLE/usr/lib/$TARGET_TRIPLE/pkgconfig/ ./build-autotools-based.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE" "$PWD/libxcb-cursor"