#!/bin/bash
#-------------------------------------------------------------------------------
# Elpida - Benchmark library
#
# Copyright (C) 2020  Ioannis Panagiotopoulos
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>
#-------------------------------------------------------------------------------

mkdir -p ./AppDir/usr/share/icons/default/apps/32/
cp -u ./images/Elpida-icon-minimal-circle-bg.svg ./AppDir/usr/share/icons/default/apps/32/elpida.svg
mkdir -p ./AppDir/usr/share/metainfo/
cp -u ./resources/dev.elpida.qt.metainfo.xml ./AppDir/usr/share/metainfo/dev.elpida.qt.metainfo.xml
cp -u ./resources/dev.elpida.qt.metainfo.xml ./AppDir/usr/share/metainfo/dev.elpida.qt.appdata.xml
appimage-builder --skip-tests

