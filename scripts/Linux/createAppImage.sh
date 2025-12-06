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

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <appdir> <relative-executable> <version> <name>"
    exit 1
fi

INSTALL_DIR=$1
EXECUTABLE=$2
VERSION=$3
NAME=$4

if [[ -z $EXECUTABLE ]]; then
  EXECUTABLE="bin/elpida-qt"
fi

if [[ -z $VERSION ]]; then
  VERSION="0.0.0.1000"
fi

if [[ -z $NAME ]]; then
  VERSION="Elpida.appimage"
fi

this_dir="$(readlink -f "$(dirname "$0")")"
mkdir -p "$INSTALL_DIR/usr/share/icons/default/apps/32/" "$INSTALL_DIR/usr/share/metainfo/" "$INSTALL_DIR/usr/share/applications/"
cp -u "images/Elpida_Icon.svg" "$INSTALL_DIR/usr/share/icons/default/apps/32/elpida.svg"
cp -u "images/Elpida_Icon.svg" "$INSTALL_DIR/elpida.svg"
cp -u "resources/dev.elpida.qt.metainfo.xml" "$INSTALL_DIR/usr/share/metainfo/dev.elpida.qt.metainfo.xml"
cp -u "resources/dev.elpida.qt.metainfo.xml" "$INSTALL_DIR/usr/share/metainfo/dev.elpida.qt.appdata.xml"
cp -u "resources/dev.elpida.qt.desktop" "$INSTALL_DIR/usr/share/applications/"
cp -u "resources/dev.elpida.qt.desktop" "$INSTALL_DIR"
CURDIR=$PWD
cd "$INSTALL_DIR" || exit
rm elpida
ln -s $EXECUTABLE elpida
cd "$CURDIR" || exit
cp -u "$this_dir/AppRun" "$INSTALL_DIR"
./appimagetool "$INSTALL_DIR" $NAME