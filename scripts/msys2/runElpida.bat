@echo off
set MSYS_FOLDER="msys2"
echo %PROCESSOR_ARCHITECTURE% | find /i "x86" > nul
if %errorlevel%==0 (
    set MSYSTEM=MINGW32
    set MINGW_FOLDER="mingw32"
    set ARCH="i686"
    set ARCH_BITS="32"
) else (
    set MSYSTEM=MINGW64
    set MINGW_FOLDER="mingw64"
    set ARCH="x86_64"
    set ARCH_BITS="64"
)

%MSYS_FOLDER%\usr\bin\mintty.exe -h alw "/usr/bin/bash.exe" --login ../../runElpida.sh