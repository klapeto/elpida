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

if [ -s "$INSTALL_PREFIX/lib/libQt6Core.a" ]; then
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

QT_CONFIG_ARGS="-static -no-shared -release -no-sbom -submodules qtcharts,qtsvg,qtbase -nomake examples -nomake tests -nomake benchmarks -nomake manual-tests -nomake minimal-static-tests"
QT_CONFIG_ARGS="$QT_CONFIG_ARGS -prefix $INSTALL_PREFIX"

if [ "$TARGET_TRIPLE" != "$HOST_TRIPLE" ] || [ -n "$FORCE_NON_HOST" ]; then
    QT_CONFIG_ARGS="$QT_CONFIG_ARGS -qt-host-path /opt/sysroots/$HOST_TRIPLE/usr"
    QT_OS="$TARGET_OS"
fi

../configure $QT_CONFIG_ARGS -- \
    -DCMAKE_C_COMPILER="$CC" \
    -DCMAKE_CXX_COMPILER="$CXX" \
    -DCMAKE_ASM_COMPILER="$CC" \
    -DCMAKE_C_FLAGS_INIT="$CFLAGS" \
    -DCMAKE_CXX_FLAGS_INIT="$CFLAGS" \
    -DCMAKE_LINKER_TYPE=LLD \
    -DCMAKE_SYSTEM_NAME="$QT_OS" \
    -DCMAKE_SYSTEM_PROCESSOR="$TARGET_ARCH" \
    -DCMAKE_SYSROOT="$SYSROOT" \
    -DCMAKE_FIND_ROOT_PATH="$SYSROOT" \
    -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
    -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
    -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
    -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY

# extreme qt workaround because configure includes system headers
mv /usr/include /usr/_include

cmake --build . --parallel

mv /usr/_include /usr/include
cmake --install .
rm -rf ./*
cd ../..