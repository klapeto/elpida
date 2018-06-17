@echo off

call setVariables.cmd

%MSYS_FOLDER%\usr\bin\bash.exe --login  -c "../../buildEnvironment.sh %ARCH% %MINGW_FOLDER%"