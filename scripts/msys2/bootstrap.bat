@echo off

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

call setVariables.cmd

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
xcopy /f /e /y bash\* %MSYS_FOLDER%
%MSYS_FOLDER%\usr\bin\bash.exe --login  -c "../../bootstrap.sh %ARCH% %MINGW_FOLDER%"
pause
exit

:no
echo Aborting...
pause 
