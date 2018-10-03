libName = 'asmlib'
if run_command('[', '-f', 'asmlib/asmlib.h', ']').returncode() != 0
    message('Downloading ' + libName + ' sources...')
	command = run_command('wget', 'http://www.agner.org/optimize/asmlib.zip')
	if command.returncode() !=0
		error('Failed to download ' + libName + ' sources: ' + command.stderr().strip())
	endif
	
	command = run_command('unzip', '-o', 'asmlib.zip', '-d', 'asmlib')
	if command.returncode() !=0
		error('Failed to download ' + libName + ' sources: ' + command.stderr().strip())
	endif
	message('Download complete.')
endif

if build_machine.cpu_family() == 'x86_64'
	if build_machine.system() == 'linux'
		asmlib = files('asmlib/libaelf64o.a')
	else
		if build_machine.system() == 'windows'
			asmlib = files('asmlib/libacof64o.lib')
		endif
	endif
else
	if build_machine.cpu_family() == 'x86'
		if build_machine.system() == 'linux'
			asmlib = files('asmlib/libaelf32o.a')
		else
			if build_machine.system() == 'windows'
				asmlib = files('asmlib/libacof32o.lib')
			endif
		endif
	endif
endif

