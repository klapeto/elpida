@echo off

call setVariables.cmd

%MSYS_FOLDER%\usr\bin\bash.exe --login  -c "../../rebuildElpida.sh"