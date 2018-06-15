#!/bin/bash
git clone https://github.com/klapeto/elpida.git
echo Downloading Elpida
cd elpida/libs

echo Downloading libpng
git clone git://git.code.sf.net/p/libpng/code libpng
cp buildfiles/libpng.build libpng/meson.build
cp buildfiles/libpng.h.meson libpng/config.h.meson
cd libpng
./autogen.sh
./configure
make pnglibconf.h
cd ..

echo Downloading zlib
git clone https://github.com/madler/zlib.git zlib
cp buildfiles/zlib.build zlib/meson.build

echo Downloading Agner Fog Asmlib
wget http://www.agner.org/optimize/asmlib.zip
unzip -o asmlib.zip -d AgnerFog/asmlib

cd ..
meson builddir
cd builddir
ninja