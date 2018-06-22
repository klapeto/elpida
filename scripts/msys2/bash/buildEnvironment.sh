#!/bin/bash
echo Downloading required packages
pacman -Syuu --noconfirm
pacman -S --noconfirm --needed git base-devel unzip mingw-w64-$1-toolchain mingw-w64-$1-python3 mingw-w64-$1-ninja mingw-w64-$1-meson
# git clone https://github.com/mesonbuild/meson.git
# cd meson
# python3 setup.py install
# ln -s /$2/bin/meson.py /$2/bin/meson
# ln -s /$2/bin/ninja /usr/bin/ninja.exe

cd ~
git clone https://github.com/klapeto/elpida.git elpida
/downloadLibrariesSources.sh

echo Done! You can close this window now and run "rebuildElpida.bat"