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

installDir=$1 || printHelpAndExit
rootDir=$2 || printHelpAndExit
major=$3 || major=0
minor=$4 || minor=0
revision=$5 || revision=1
build=$6 || build=1000

# Fix version on AppImageBuilder.yml
cp ./AppImageBuilder.yml ./AppImageBuilder.yml.old
sed  s/"version: latest"/"version: $major.$minor.$revision.$build"/ < ./AppImageBuilder.yml.old > ./AppImageBuilder.yml

mkdir -p "$installDir/usr/share/icons/default/apps/32/" \
  "$installDir/usr/share/metainfo/" \
  "$installDir/usr/share/applications/"

cp -u "$rootDir/images/Elpida_Icon.svg" "$installDir/usr/share/icons/default/apps/32/elpida.svg"
cp -u "$rootDir/resources/dev.elpida.qt.metainfo.xml" "$installDir/usr/share/metainfo/dev.elpida.qt.metainfo.xml"
cp -u "$rootDir/resources/dev.elpida.qt.metainfo.xml" "$installDir/usr/share/metainfo/dev.elpida.qt.appdata.xml"
cp -u "$rootDir/resources/dev.elpida.qt.desktop" "$installDir/usr/share/applications/"

appimage-builder --skip-tests

function printHelpAndExit() {
   echo "Usage: ./createAppImage 'Installed directory' 'root repo directory'"
   exit
}
