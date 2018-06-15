if exist "msys64" (
	set MSYSTEM=MINGW64
	set MSYSPATH="msys64"
	set ARCH="x86_64"
) else (
	if exist "msys32" (
		set MSYSTEM=MINGW32
		set MSYSPATH="msys32"
		set ARCH="i686"
	) else (
		echo Error! No MSYS folder found. Consider downloading it. 1>&2
		exit /b 0
	)
)
%MSYSPATH%\usr\bin\mintty.exe "/usr/bin/bash.exe" --login ../../install.sh %ARCH% %MSYSPATH%
%MSYSPATH%\usr\bin\mintty.exe "/usr/bin/bash.exe" --login ../../buildElpida.sh
pause