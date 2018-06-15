#!/bin/bash
pacman -Syuu --noconfirm
pacman -S --noconfirm --needed git base-devel unzip mingw-w64-$1-toolchain mingw-w64-$1-python3 mingw-$1-ninja
git clone https://github.com/mesonbuild/meson.git
cd meson
python3 setup.py install
ln -s /$2/bin/meson.py /$2/bin/meson
ls -s /$2/bin/ninja /usr/bin/ninja.exe
