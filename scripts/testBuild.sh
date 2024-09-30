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

export TARGET_ARCH="amd64"
export BUILD_DIR="./build"
export INSTALL_DIR="./AppDir"
export ELPIDA_VERSION_BUILD="0000"
export ELPIDA_VERSION_STRING="0.0.0"
export PACKAGE_PREFIX="Elpida"
export PACKAGE_SEPARATOR="-"
export PACKAGE_FILENAME="$PACKAGE_PREFIX-$ELPIDA_VERSION_STRING-$TARGET_ARCH"
export PACKAGE_FILENAME_DEPLOY="$PACKAGE_PREFIX-latest-$TARGET_ARCH"
export PACKAGE_FILENAME_DEPLOY_BETA="$PACKAGE_PREFIX-latest-beta-$TARGET_ARCH"
export PACKAGE_QT_SUFFIX="Qt"
export PACKAGE_CLI_SUFFIX="Cli"
export DEPLOY_ENVIRONMENT="development"

if [ ! -f ./appimage-builder ]; then
  wget -O appimage-builder https://github.com/AppImageCrafters/appimage-builder/releases/download/v1.1.0/appimage-builder-1.1.0-x86_64.AppImage
  chmod +x appimage-builder
fi

rm *.SHA256SUMS
rm *.zsync
rm Elpida*.AppImage
rm -rf $INSTALL_DIR
rm -rf $BUILD_DIR
mkdir $INSTALL_DIR $BUILD_DIR
cmake -S . -B $BUILD_DIR -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
make -C build -j$(nproc) install
mkdir -p "$INSTALL_DIR/usr/share/icons/default/apps/32/" "$INSTALL_DIR/usr/share/metainfo/" "$INSTALL_DIR/usr/share/applications/"
cp -u "images/Elpida_Icon.svg" "$INSTALL_DIR/usr/share/icons/default/apps/32/elpida.svg"
cp -u "resources/dev.elpida.qt.metainfo.xml" "$INSTALL_DIR/usr/share/metainfo/dev.elpida.qt.metainfo.xml"
cp -u "resources/dev.elpida.qt.metainfo.xml" "$INSTALL_DIR/usr/share/metainfo/dev.elpida.qt.appdata.xml"
cp -u "resources/dev.elpida.qt.desktop" "$INSTALL_DIR/usr/share/applications/"
./appimage-builder --skip-tests --recipe AppImageBuilder.cli.$TARGET_ARCH.yml
./appimage-builder --skip-tests --recipe AppImageBuilder.qt.$TARGET_ARCH.yml
sha256sum "$PACKAGE_FILENAME.$PACKAGE_QT_SUFFIX.AppImage" > $PACKAGE_FILENAME.$PACKAGE_QT_SUFFIX.SHA256SUMS
sha256sum "$PACKAGE_FILENAME.$PACKAGE_CLI_SUFFIX.AppImage" > $PACKAGE_FILENAME.$PACKAGE_CLI_SUFFIX.SHA256SUMS
cp "$PACKAGE_FILENAME.$PACKAGE_QT_SUFFIX.AppImage" "$PACKAGE_FILENAME_DEPLOY.$PACKAGE_QT_SUFFIX.AppImage"
cp "$PACKAGE_FILENAME.$PACKAGE_CLI_SUFFIX.AppImage" "$PACKAGE_FILENAME_DEPLOY.$PACKAGE_CLI_SUFFIX.AppImage"
sha256sum "$PACKAGE_FILENAME.$PACKAGE_QT_SUFFIX.AppImage" > $PACKAGE_FILENAME.$PACKAGE_QT_SUFFIX.AppImage.SHA256SUMS
sha256sum "$PACKAGE_FILENAME.$PACKAGE_CLI_SUFFIX.AppImage" > $PACKAGE_FILENAME.$PACKAGE_CLI_SUFFIX.AppImage.SHA256SUMS
sha256sum "$PACKAGE_FILENAME_DEPLOY.$PACKAGE_QT_SUFFIX.AppImage" > $PACKAGE_FILENAME_DEPLOY.$PACKAGE_QT_SUFFIX.AppImage.SHA256SUMS
sha256sum "$PACKAGE_FILENAME_DEPLOY.$PACKAGE_CLI_SUFFIX.AppImage" > $PACKAGE_FILENAME_DEPLOY.$PACKAGE_CLI_SUFFIX.AppImage.SHA256SUMS