project('elpida', 'cpp','c', 
version: '0.0.1',
license: 'GPL3',
default_options : ['c_std=c11', 'cpp_std=c++11'])

add_global_arguments('-fno-builtin', language : 'c')
add_global_arguments('-fno-builtin', language : 'cpp')

gcc = meson.get_compiler('c')
gpp = meson.get_compiler('cpp')

inc = include_directories('src')
libsInclude = include_directories('libs')
configuration_inc = include_directories('.')

thread_dep = dependency('threads')

subdir('libs')
subdir('src')


elpida = library('elpida',
			include_directories : [inc, configuration_inc],
			dependencies : thread_dep,
			link_whole: [core, generalTasks, imageTasks, imageEncoders, utilities],
			install: true)
			
executable('elpida', 'src/main.cpp',
			link_with: elpida,
			include_directories : [inc, configuration_inc])
