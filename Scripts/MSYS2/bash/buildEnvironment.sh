#!/bin/bash
#-------------------------------------------------------------------------------
# Elpida - Benchmark library
# 
# Copyright (C) 2018  Ioannis Panagiotopoulos
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

echo Downloading required packages
pacman -Syuu --noconfirm
pacman -S --noconfirm --needed git base-devel unzip mingw-w64-$1-toolchain mingw-w64-$1-python3 mingw-w64-$1-ninja mingw-w64-$1-meson mingw-w64-$1-gtest
# git clone https://github.com/mesonbuild/meson.git
# cd meson
# python3 setup.py install
# ln -s /$2/bin/meson.py /$2/bin/meson
# ln -s /$2/bin/ninja /usr/bin/ninja.exe

cd ~
git clone https://gitlab.com/dev-hood/elpida/elpida.git elpida
/downloadLibrariesSources.sh

echo Done! You can close this window now and run "rebuildElpida.bat"
