libName = 'hwloc'
libRepo = 'https://github.com/open-mpi/hwloc'
if run_command('[', '-d', libName, ']').returncode() != 0
    message('Downloading ' + libName + ' sources...')
	download = run_command('git', 'clone', libRepo, libName)
	if download.returncode() !=0
		error('Failed to clone ' + libName + ' sources: ' + download.stderr().strip())
	endif
	message('Download complete.')
	message('Configuring' + libName)
	configCommand = run_command('sh', '-c', 'cd '+ libName + ' && ./autogen.sh && ./configure --prefix=$PWD/install --enable-static --disable-shared LDFLAGS="-static"')
	if configCommand.returncode() != 0
		error('Failed to configure '+ libName + build.stderr())
	endif
else 
	message('Updating ' + libName + ' sources...')
	download = run_command('sh', '-c', 'cd '+ libName + ' && git pull')
	if download.returncode() != 0
		error('Failed to update ' + libName + ' sources: ' + download.stderr().strip())
	endif
endif

#message('Building '+ libName)
#build = run_command('sh', '-c', 'cd '+ libName + ' && make && make install')

#if build.returncode() != 0
#	error('Failed to build '+ libName + build.stderr())
#endif

hwlocInclude = include_directories('hwloc/include')
hwlocInternalInclude = include_directories('hwloc/hwloc')

hwlocDeps = []
hwlocSources = [
        'hwloc/hwloc/topology.c',
        'hwloc/hwloc/traversal.c',
       	'hwloc/hwloc/distances.c',
        'hwloc/hwloc/components.c',
        'hwloc/hwloc/bind.c',
        'hwloc/hwloc/bitmap.c',
        'hwloc/hwloc/pci-common.c',
        'hwloc/hwloc/diff.c',
        'hwloc/hwloc/shmem.c',
        'hwloc/hwloc/misc.c',
        'hwloc/hwloc/base64.c',
        'hwloc/hwloc/topology-noos.c',
        'hwloc/hwloc/topology-synthetic.c',
        'hwloc/hwloc/topology-xml.c',
        'hwloc/hwloc/topology-xml-nolibxml.c',
        'hwloc/hwloc/topology-x86.c'
]

if build_machine.system() == 'linux'
	hwlocSources+='hwloc/hwloc/topology-linux.c'
	hwlocSources+='hwloc/hwloc/topology-hardwired.c'
else
	if build_machine.system() == 'windows'
		hwlocSources+='hwloc/hwloc/topology-windows.c'
	endif
endif

pDep = dependency('pciaccess', required : false)
if pDep.found()
	hwlocDeps+= pDep
	hwlocSources+='hwloc/hwloc/topology-pci.c'
endif

pDep = dependency('opencl', required : false)
if pDep.found()
	hwlocDeps+= pDep
endif

pDep = dependency('pciaccess', required : false)
if pDep.found()
	hwlocDeps+= pDep
endif

hwloc = static_library('hwloc', hwlocSources, 
include_directories: [hwlocInclude, hwlocInternalInclude], 
dependencies: hwlocDeps,
c_args : ['-DRUNSTATEDIR=""', '-DHWLOC_PLUGINS_PATH=""'])

						
