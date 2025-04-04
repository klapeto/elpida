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

if [! -f "86_64-w64-mingw32.static-cmake"]
then
  alias cmake=86_64-w64-mingw32.static-cmake
fi

git clone https://gitlab.com/dev-hood/elpida/elpida.git && cd elpida && git checkout staging && git submodule update --init && mkdir build && cd build