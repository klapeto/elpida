#!/bin/bash
if [ ! -d "~/elpida" ]; then
    echo Elpida source was not found. Pleaste ensure that "buildEnvironment.bat" was run first and there were no errors
    exit    
fi
cd ~/edpida
git clean -fd
rm -r builddir
meson builddir
cd builddir
ninja
cp elpida.exe ../
cp libelpida.dll ../
