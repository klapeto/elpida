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

buildDir=$1 || buildDir="build"
installDir=$2 || installDir="AppDir"
apiKey=$3 || apiKey="dummyKey"
major=$4 || major=0
minor=$5 || minor=0
revision=$6 || revision=1
build=$7 || build=1000

git submodule update --init

x86_64-w64-mingw32.shared-cmake -S . \
  -B "$buildDir" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$installDir" \
  -DELPIDA_WEB_API_KEY="$apiKey" \
  -DELPIDA_MAJOR=$major \
  -DELPIDA_MINOR=$minor \
  -DELPIDA_REVISION=$revision \
  -DELPIDA_BUILD=$build

make -C "$buildDir" -j$(nproc) install