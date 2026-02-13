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

if [ -s "$INSTALL_PREFIX/lib/libQt6Core.a" ] || [ -s "$INSTALL_PREFIX/lib/libQt6Core.so" ]; then
  echo "Seems already built. We will not build it again"
  exit 0;
fi

if [ ! -d ./qt6 ]; then
    git clone --depth=1 -b v6.10.0 git://code.qt.io/qt/qt5.git qt6
    cd qt6
    ./init-repository --module-subset=qtcharts,qtsvg,qtbase
    cd ..
fi

mkdir -p qt6/build
cd qt6/build
rm -rf ./*

QT_CONFIG_ARGS="-static -no-shared -release -no-sbom -force-bundled-libs"
QT_CONFIG_ARGS="$QT_CONFIG_ARGS -prefix $INSTALL_PREFIX"

: ${QT_HOST_PATH:="/opt/sysroots/$HOST_TRIPLE/usr"}

if [ "$TARGET_TRIPLE" != "$HOST_TRIPLE" ] || [ -n "$FORCE_NON_HOST" ] && [ -z "$FORCE_HOST" ]; then
    QT_CONFIG_ARGS="$QT_CONFIG_ARGS -qt-host-path $QT_HOST_PATH"
    QT_OS="$TARGET_OS"
fi

case $TARGET_TRIPLE in
*-linux-gnu*)
    # force configuration to fail we cannot get xcb (qt wont even start without it)
    QT_CONFIG_ARGS="$QT_CONFIG_ARGS -xcb"
    ;;
esac

../configure $QT_CONFIG_ARGS -- \
    -DCMAKE_TOOLCHAIN_FILE="/tmp/cross.linux.cmake"

cmake --build . --parallel
cmake --install .
rm -rf ./*
cd ../..