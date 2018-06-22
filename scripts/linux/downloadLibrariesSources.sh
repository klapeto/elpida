#!/bin/bash
echo Downloading zlib
cd libs
git clone https://github.com/madler/zlib.git zlib
cp buildfiles/zlib.build zlib/meson.build

echo Downloading libpng
git clone git://git.code.sf.net/p/libpng/code libpng
cp buildfiles/libpng.build libpng/meson.build
cp buildfiles/libpng.h.meson libpng/config.h.meson
cp libpng/scripts/pnglibconf.h.prebuilt libpng/pnglibconf.h

echo Downloading Agner Fog Asmlib
wget http://www.agner.org/optimize/asmlib.zip
unzip -o asmlib.zip -d AgnerFog/asmlib
cd ..
