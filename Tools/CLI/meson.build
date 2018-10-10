cli_sources = [
	'Main.cpp',
	'CLIOptions.cpp',
	'Interpreter.cpp',
	'InterpreterCommand.cpp',
	'TaskBatchLoader.cpp',
	'CommandRegistry.cpp',
	'CommandNotFoundException.cpp',
	'Commands/Quit.cpp',
	'Commands/Cpuid.cpp',
	'Commands/Help.cpp',
	'Commands/TaskBatchCommand.cpp',
	'Commands/ListBatches.cpp'
]

cli_inlcudes = include_directories('.')

executable('elpida-cli', cli_sources,
			link_with: elpida,
			include_directories : [rootIncude, cli_inlcudes],
			build_rpath: libraryInstallPath + ':' + pluginInstallDir,
			install_rpath: libraryInstallPath + ':' + pluginInstallDir,
			install: true)
			
