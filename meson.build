project('elpida', 'cpp','c', 
version: '0.0.1',
license: 'GPL3',
default_options : ['c_std=c11', 'cpp_std=c++11'])

add_global_arguments('-fno-builtin', language : 'c')
add_global_arguments('-fno-builtin', language : 'cpp')

gcc = meson.get_compiler('c')
gpp = meson.get_compiler('cpp')

rootIncude = include_directories('.')
libsInclude = include_directories('Libraries')

thread_dep = dependency('threads')

if build_machine.system() == 'linux'
	dl_dep = gpp.find_library('dl', required: true)
else
	if build_machine.system() == 'windows'
		dl_dep = gpp.find_library('kernel32', required: true)
	endif
endif

install_subdir('Resources', install_dir : 'bin/Elpida')

installationPrefix = get_option('prefix')
libraryInstallPath = join_paths(installationPrefix, get_option('libdir'))
binaryInstallPath = join_paths(installationPrefix, get_option('bindir'))

enable_optimizations = false

if enable_optimizations
	defaultCppArgs = ['-march=native', '-O3', '-funroll-loops', '-msse3', '-msse4.1', '-masm=intel']
else
	defaultCppArgs = ['-O0', '-g3', '-msse3', '-msse4.1', '-masm=intel']
endif

qt5 = import('qt5')
qt5_dep = dependency('qt5', modules: ['Widgets', 'Charts'], required: false)

pluginInstallDir = join_paths(binaryInstallPath, 'Elpida')

subdir('Libraries')
subdir('Elpida')
subdir('TaskBatches')
subdir('Tools')

enable_tests = true

if enable_tests 
	subdir('Tests')
endif