#!/bin/bash
cd ~
if [ ! -d "elpida" ]; then
    echo Elpida source was not found. Pleaste ensure that "buildEnvironment.bat" was run first and there were no errors
    exit    
fi
cd ~/elpida
meson builddir
cd builddir
ninja
cp elpida.exe ../
cp libelpida.dll ../
echo Done! You can now run "runElpida.bat" to execute elpida