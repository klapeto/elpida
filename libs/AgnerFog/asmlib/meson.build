if build_machine.cpu_family() == 'x86_64'
	if build_machine.system() == 'linux'
		asmlib = files('libaelf64o.a')
	else
		if build_machine.system() == 'windows'
			asmlib = files('libacof64o.lib')
		endif
	endif
else
	if build_machine.cpu_family() == 'x86'
		if build_machine.system() == 'linux'
			asmlib = files('libaelf32o.a')
		else
			if build_machine.system() == 'windows'
				asmlib = files('libacof32o.lib')
			endif
		endif
	endif
endif

