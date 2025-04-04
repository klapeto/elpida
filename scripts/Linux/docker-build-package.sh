#!/bin/bash
#
#
#  Copyright (c) 2024  Ioannis Panagiotopoulos
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

export ELPIDA_ARCH=amd64
export INSTALL_DIR=./AppDir
export ELPIDA_VERSION_STRING=1.0.0
export PACKAGE_FILENAME=Elpida.cli
export PACKAGE_CLI_SUFFIX=$ELPIDA_ARCH
git clone https://gitlab.com/dev-hood/elpida/elpida.git
cd elpida || exit
git checkout staging
git submodule update --init
mkdir build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR"
make -C build -j$(nproc) install
mkdir -p "$INSTALL_DIR/usr/share/icons/default/apps/32/" "$INSTALL_DIR/usr/share/metainfo/" "$INSTALL_DIR/usr/share/applications/"
cp -u "images/Elpida_Icon.svg" "$INSTALL_DIR/usr/share/icons/default/apps/32/elpida.svg"
cp -u "resources/dev.elpida.qt.metainfo.xml" "$INSTALL_DIR/usr/share/metainfo/dev.elpida.qt.metainfo.xml"
cp -u "resources/dev.elpida.qt.metainfo.xml" "$INSTALL_DIR/usr/share/metainfo/dev.elpida.qt.appdata.xml"
cp -u "resources/dev.elpida.qt.desktop" "$INSTALL_DIR/usr/share/applications/"
appimage-builder --skip-tests --recipe AppImageBuilder.cli.$ELPIDA_ARCH.yml
