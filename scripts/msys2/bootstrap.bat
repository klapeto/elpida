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

echo ====================================
echo ^|                                  ^|
echo ^|              Elpida              ^|
echo ^|                                  ^|
echo ====================================
echo.
echo Welcome to Elpida!
echo These scripts will setup the necessary environment required
echo to build Elpida from sources, as well as building itself.
echo To do so, some things must be downloaded and extracted here.
echo.
echo    1. MSYS2 build platform (https://www.msys2.org)
echo    2. Required packages
echo    3. Elpida sources
echo    4. Sources from libraries used from Elpida
echo.
echo All these are handled automatically by these scripts. Anything
echo downloaded and extracted will be located in the folder that this
echo script is run. Nothing is installed system-wise. MSYS2 is 
echo extracted with it's installation executable by modifying it's
echo behaviour so that it just extracts the files here. This is done
echo by calling it and providing a Javascript file that overrides
echo some fucntions.
echo This initial environment setup will take some time to download and
echo and extract. After this, it will not be needed again. All you needed
echo after this is running "runElpida.bat"
echo.
echo IMPORTANT! These scripts will download more than 300MB of data.
echo Please keep this in mind if you are in a metered internet connection.
echo IMPORTANT! The setup requires at least 2GB of disk space.
echo.

:confirm
set /P c=Are you sure you want to continue[Y/N]?
if /I "%c%" EQU "Y" goto :yes
if /I "%c%" EQU "N" goto :no
goto :choice


:yes
call getMSYS2.bat
xcopy /f /e /y scripts\* %MSYS_FOLDER%
%MSYS_FOLDER%\usr\bin\mintty.exe -h alw "/usr/bin/bash.exe" --login ../../bootstrap.sh %ARCH% %MINGW_FOLDER%
exit

:no
echo Aborting...
pause 
