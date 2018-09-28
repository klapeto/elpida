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

if [ -f /etc/lsb-release ]; then
    #Debian
    sudo apt install git g++ meson ninja-build unzip libgtest-dev
elif [ -f /etc/redhat-release ]; then
    # Fedora
    sudo dnf install git gcc-c++ meson ninja-build unzip gtest-devel
else
	echo This linux distribution is unsupported by this script. You have to manually install these packages: git g++ 4.8+ meson ninja unzip
fi