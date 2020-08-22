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

if [ -f /etc/lsb-release ]; then
    #Debian
    export DEBIAN_FRONTEND=noninteractive
    apt install -y python3-pip python3-setuptools patchelf desktop-file-utils libgdk-pixbuf2.0-dev

    # Install appimagetool AppImage
    wget https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage -O /usr/local/bin/appimagetool
    chmod +x /usr/local/bin/appimagetool
    pip3 install appimage-builder
else
	echo This linux distribution is unsupported by this script. You have to manually install these packages: git g++ 4.8+ cmake automake libtool pkg-config qt5
fi