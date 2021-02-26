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


buildDir=$1 || printHelpAndExit
installDir=$2 || printHelpAndExit
filenamePrefix=$3 || printHelpAndExit
filenameSuffix=$4 || printHelpAndExit

currentDir=$(pwd)
stagingDir=$(mktemp -d -t ci-XXXXXXXXXX)

# Copy all dependencies from both install and build directories as well as the runtime/platform binaries
/mxe/tools/copydlldeps.sh -c \
  -f "$installDir/bin/elpida-qt.exe" \
  -S "/mxe/usr/x86_64-w64-mingw32.shared/bin/ /mxe/usr/x86_64-w64-mingw32.shared/qt5/bin/" \
  -R "$installDir  $buildDir" \
  -d "$stagingDir"

# Copy necessary Qt plugins not detected by the copydlldeps script
cp -r /mxe/usr/x86_64-w64-mingw32.shared/qt5/plugins/iconengines "$stagingDir"
cp -r /mxe/usr/x86_64-w64-mingw32.shared/qt5/plugins/imageformats "$stagingDir"
cp -r /mxe/usr/x86_64-w64-mingw32.shared/qt5/plugins/platforms "$stagingDir"
cp -r /mxe/usr/x86_64-w64-mingw32.shared/qt5/plugins/platformthemes "$stagingDir"
cp -r /mxe/usr/x86_64-w64-mingw32.shared/qt5/plugins/styles "$stagingDir"
cp /mxe/usr/x86_64-w64-mingw32.shared/qt5/bin/Qt5Svg.dll "$stagingDir"

# Create the directory for benchmarks
mkdir "$stagingDir/Benchmarks"

# For each benchmark found, copy it to new directory
find "$buildDir/src/Benchmarks" -type f -name '*.dll' -exec cp "{}" "$stagingDir/Benchmarks" \;

# Copy the entry point binary
cp "$installDir/bin/elpida-qt.exe" "$stagingDir"

# Package it to zip
cd "$stagingDir"
zip z -r .
cp ./z.zip "$currentDir/$filenamePrefix$filenameSuffix"
cd "$currentDir"

rm -rf "$stagingDir"

function printHelpAndExit() {
   echo "Usage: ./packageBinaries 'Build directory' 'Installed directory' 'output Filename'"
   exit
}
