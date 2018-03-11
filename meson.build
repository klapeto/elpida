project('elpida', 'cpp', 
version: '0.0.1',
license: 'GPL3',
default_options : ['cpp_std=c++11'])

inc = include_directories('src')
subdir('src')
executable('elpida', 'src/main.cpp',
			include_directories : inc,
			link_with: [runner])
