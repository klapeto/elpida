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

INSTALL_DIR=$1 || ./AppDir
EXECUTABLE=$2 || bin/elpida-qt
VERSION=$3 || "0.0.0.1000"
NAME=$4 || elpida.appimage

this_dir="$(readlink -f "$(dirname "$0")")"
mkdir -p "$INSTALL_DIR/usr/share/icons/default/apps/32/" "$INSTALL_DIR/usr/share/metainfo/" "$INSTALL_DIR/usr/share/applications/"
cp -u "images/Elpida_Icon.svg" "$INSTALL_DIR/usr/share/icons/default/apps/32/elpida.svg"
cp -u "images/Elpida_Icon.svg" "$INSTALL_DIR/elpida.svg"
cp -u "resources/dev.elpida.qt.metainfo.xml" "$INSTALL_DIR/usr/share/metainfo/dev.elpida.qt.metainfo.xml"
cp -u "resources/dev.elpida.qt.metainfo.xml" "$INSTALL_DIR/usr/share/metainfo/dev.elpida.qt.appdata.xml"
cp -u "resources/dev.elpida.qt.desktop" "$INSTALL_DIR/usr/share/applications/"
cp -u "resources/dev.elpida.qt.desktop" "$INSTALL_DIR"
CURDIR=$PWD
cd $INSTALL_DIR
rm elpida
ln -s $EXECUTABLE elpida
cd $CURDIR
cp -u "$this_dir/AppRun" "$INSTALL_DIR"
appimagetool "$INSTALL_DIR" $NAME