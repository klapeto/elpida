libName = 'zlib'
libRepo = 'https://github.com/madler/zlib.git'
if run_command('[', '-d', libName, ']').returncode() != 0
    message('Downloading ' + libName + ' sources...')
	download = run_command('git', 'clone', libRepo, libName)
	if download.returncode() !=0
		error('Failed to clone ' + libName + ' sources: ' + download.stderr().strip())
	endif
	message('Download complete.')
else 
	message('Updating ' + libName + ' sources...')
	download = run_command('sh', '-c', 'cd '+ libName + ' && git pull')
	if download.returncode() != 0
		error('Failed to update ' + libName + ' sources: ' + download.stderr().strip())
	endif
endif

localCppArgs = ['-march=native', '-O3', '-funroll-loops']

zlib = static_library('zlib', 
						['zlib/adler32.c',
						'zlib/compress.c',
						'zlib/crc32.c',
						'zlib/deflate.c',
						'zlib/gzclose.c',
						'zlib/gzlib.c',
						'zlib/gzread.c',
						'zlib/gzwrite.c',
						'zlib/infback.c',
						'zlib/inffast.c',
						'zlib/inflate.c',
						'zlib/inftrees.c',
						'zlib/trees.c',
						'zlib/uncompr.c',
						'zlib/zutil.c'],
						cpp_args: localCppArgs,
						c_args: localCppArgs,
						install : false)
						
zlibInclude = include_directories('zlib')