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

cd ~
if [ ! -d "elpida" ]; then
    echo Elpida source was not found. Please ensure that "buildEnvironment.bat" was run first and there were no errors
    exit
fi

if [ ! -d "elpida/build" ]; then
  mkdir "elpida/build"
fi

cd ~/elpida/build
cmake -G "MSYS Makefiles" -DCMAKE_INSTALL_PREFIX=$PWD/../install ..
make -j$(nproc)
make install
echo Done! You can now run "runElpida.bat" to execute elpida
