#!/bin/bash
echo Downloading required packages
pacman -Syuu --noconfirm
pacman -S --noconfirm --needed git base-devel unzip mingw-w64-$1-toolchain mingw-w64-$1-python3 mingw-w64-$1-ninja
git clone https://github.com/mesonbuild/meson.git
cd meson
python3 setup.py install
ln -s /$2/bin/meson.py /$2/bin/meson
ln -s /$2/bin/ninja /usr/bin/ninja.exe

echo Downloading Elpida
cd ~
git clone https://github.com/klapeto/elpida.git elpida
cd elpida
git checkout master
git pull

echo Downloading libpng
cd ~/elpida/libs
git clone git://git.code.sf.net/p/libpng/code libpng
cp buildfiles/libpng.build libpng/meson.build
cp buildfiles/libpng.h.meson libpng/config.h.meson
cd libpng
./autogen.sh
./configure
make pnglibconf.h

echo Downloading zlib
cd ~/elpida/libs
git clone https://github.com/madler/zlib.git zlib
cp buildfiles/zlib.build zlib/meson.build

echo Downloading Agner Fog Asmlib
cd ~/elpida/libs
wget http://www.agner.org/optimize/asmlib.zip
unzip -o asmlib.zip -d AgnerFog/asmlib
echo Done! You can close this window now and run "rebuildElpida.bat"