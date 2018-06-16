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
%MSYS_INSTALLATION_EXE% -v --script %OUTPUT_INSTALL_SCRIPT%51
echo Done!
%MSYS_FOLDER%\usr\bin\bash.exe --login -c "pacman -Syuu --noconfirm"