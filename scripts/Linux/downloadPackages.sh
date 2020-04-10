#!/bin/bashhttps://files.slack.com/files-pri/TFCSM2QV9-FSPH7NKBL/image.png
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
    sudo apt install -y git g++ cmake libgtest-dev libnuma-dev automake libtool pkg-config qtbase5-dev libqt5charts5-dev
elif [ -f /etc/redhat-release ]; then
    # Fedora
    sudo dnf install -y git gcc-c++ cmake libnuma-devel automake libtool pkg-config qtbase5-devel libqt5charts5-devel
else
	echo This linux distribution is unsupported by this script. You have to manually install these packages: git g++ 4.8+ cmake automake libtool pkg-config
fi
