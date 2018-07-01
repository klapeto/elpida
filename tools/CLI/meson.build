cli_sources = [
	'Main.cpp',
	'CLIOptions.cpp',
	'Interpreter.cpp',
	'InterpreterCommand.cpp',
	'CommandRegistry.cpp',
	'CommandNotFoundException.cpp',
	'Commands/Quit.cpp',
	'Commands/Cpuid.cpp',
	'Commands/Help.cpp'
]

cli_inlcudes = include_directories('.')

executable('elpida-cli', cli_sources,
			link_with: elpida,
			include_directories : [inc, configuration_inc, cli_inlcudes],
			build_rpath: '../lib64',
			install_rpath: '../lib64',
			install: true)
			
