REM **************************************************************************
REM Elpida -  Benchmark library
REM 
REM Copyright (C) 2018  Ioannis Panagiotopoulos
REM This program is free software: you can redistribute it and/or modify
REM it under the terms of the GNU General Public License as published by
REM the Free Software Foundation, either version 3 of the License, or
REM (at your option) any later version.
REM 
REM This program is distributed in the hope that it will be useful,
REM but WITHOUT ANY WARRANTY; without even the implied warranty of
REM MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
REM GNU General Public License for more details.
REM 
REM You should have received a copy of the GNU General Public License
REM along with this program.  If not, see <https://www.gnu.org/licenses/>
REM **************************************************************************

@echo off

call setVariables.cmd

set INPUT_INSTALL_SCRIPT_TEMPLATE="msys2_autoInstall_template.js"
set OUTPUT_INSTALL_SCRIPT="msys2_autoInstall.js"
set INSTALL_SCRIPT_MSYS_FOLDER_PLACEHOLDER="__MSYS_FOLDER__"
set INSTALL_SCRIPT_MSYS_VERSION_PLACEHOLDER="__MSYS_VERSION__"
set INSTALL_SCRIPT_ARCH_BITS_PLACEHOLDER="__ARCH_BITS__"

set MSYS_VERSION="20180531"
set MSYS_RELEASE_REPO="http://repo.msys2.org/distrib/%ARCH%/"
set MSYS_INSTALLATION_EXE="msys2-%ARCH%-%MSYS_VERSION%.exe"

echo Please wait while downloading MSYS...
powershell.exe -nologo -noprofile -command "(New-Object Net.WebClient).DownloadFile('%MSYS_RELEASE_REPO%%MSYS_INSTALLATION_EXE%', '%MSYS_INSTALLATION_EXE%')"
echo Done!
echo Generating MSYS Install script...
powershell.exe -nologo -noprofile -command "(Get-Content %INPUT_INSTALL_SCRIPT_TEMPLATE%) | ForEach-Object { $_ -replace '%INSTALL_SCRIPT_MSYS_FOLDER_PLACEHOLDER%', '%MSYS_FOLDER%' } | ForEach-Object { $_ -replace '%INSTALL_SCRIPT_ARCH_BITS_PLACEHOLDER%', '%ARCH_BITS%' } | ForEach-Object { $_ -replace '%INSTALL_SCRIPT_MSYS_VERSION_PLACEHOLDER%', '%MSYS_VERSION%' } | Set-Content %OUTPUT_INSTALL_SCRIPT%"
echo Done!
echo Extracting MSYS files...
%MSYS_INSTALLATION_EXE% -v --script %OUTPUT_INSTALL_SCRIPT%
echo Done!
%MSYS_FOLDER%\usr\bin\bash.exe --login -c "pacman -Syuu --noconfirm"