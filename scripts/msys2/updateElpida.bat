@echo off

call setVariables.cmd

%MSYS_FOLDER%\usr\bin\bash.exe --login  -c "../../updateElpida.sh %ARCH% %MINGW_FOLDER%"