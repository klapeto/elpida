libName = 'libpng'
libRepo = 'git://git.code.sf.net/p/libpng/code'
if run_command('[', '-d', libName, ']').returncode() != 0
    message('Downloading ' + libName + ' sources...')
	command = run_command('git', 'clone', libRepo, libName)
	if command.returncode() !=0
		error('Failed to clone ' + libName + ' sources: ' + command.stderr().strip())
	endif
	message('Download complete.')
else 
	message('Updating ' + libName + ' sources...')
	command = run_command('sh', '-c', 'cd '+ libName + ' && git pull')
	if command.returncode() != 0
		error('Failed to update ' + libName + ' sources: ' + command.stderr().strip())
	endif
endif

command = run_command('cp', 'libpng/scripts/pnglibconf.h.prebuilt', 'libpng/pnglibconf.h')
if command.returncode() != 0
   error('Failded to copy file' + command.stderr().strip())
endif 

cdata = configuration_data()
check_headers = [
  ['HAVE_DLFCN_H', 'dlfcn.h'],
  ['HAVE_INTTYPES_H', 'inttypes.h'],
  ['HAVE_MEMORY_H', 'memory.h'],
  ['HAVE_STDINT_H', 'stdint.h'],
  ['HAVE_STDLIB_H', 'stdlib.h'],
  ['HAVE_STRINGS_H', 'strings.h'],
  ['HAVE_STRING_H', 'string.h'],
  ['HAVE_SYS_STAT_H', 'sys/stat.h'],
  ['HAVE_SYS_TYPES_H', 'sys/types.h'],
  ['HAVE_UNISTD_H', 'unistd.h'],
]

foreach h : check_headers
  if gcc.has_header(h.get(1))
    cdata.set(h.get(0), 1)
  endif
endforeach

check_functions = [
# check token ('HAVE_LIBM',)
  ['HAVE_MEMSET', 'memset', '#include<string.h>'],
  ['HAVE_MEMSET', 'memset', '#include<string.h>'],
]

m_dep = gcc.find_library('m', required : false)

foreach f : check_functions
  if gcc.has_function(f.get(1), prefix : f.get(2))
    cdata.set(f.get(0), 1)
  endif
endforeach

if (gcc.has_function('pow', prefix: '#include<math.h>') or gcc.has_function('pow', prefix: '#include<math.h>', args: '-lm'))
	cdata.set('HAVE_POW', 1)
endif

if (gcc.has_function('feenableexcept', prefix: '#include<fenv.h>') or gcc.has_function('feenableexcept', prefix: '#include<fenv.h>', args: '-lm'))
	cdata.set('HAVE_FEENABLEEXCEPT', 1)
endif

configure_file(input: 'libpng.h.meson',
  output: 'config.h',
  configuration: cdata)


libpng = static_library('libpng', 
						['libpng/png.c',
						'libpng/pngerror.c',
						'libpng/pngget.c',
						'libpng/pngmem.c',
						'libpng/pngpread.c',
						'libpng/pngread.c',
						'libpng/pngrio.c',
						'libpng/pngrtran.c',
						'libpng/pngrutil.c',
						'libpng/pngset.c',
						'libpng/pngtrans.c',
						'libpng/pngwio.c',
						'libpng/pngwrite.c',
						'libpng/pngwtran.c',
						'libpng/pngwutil.c'],
						dependencies : m_dep,
						link_whole: zlib,
						include_directories : zlibInclude,
						c_args : ['-DHAVE_CONFIG_H', '-DHAVE_LIBZ'],
						install : false)
