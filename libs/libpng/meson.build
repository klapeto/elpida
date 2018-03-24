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


configure_file(input : 'config.h.meson',
  output : 'config.h',
  configuration : cdata)


libpng_sources = ['png.c',
				'pngerror.c',
				'pngget.c',
				'pngmem.c',
				'pngpread.c',
				'pngread.c',
				'pngrio.c',
				'pngrtran.c',
				'pngrutil.c',
				'pngset.c',
				'pngtrans.c',
				'pngwio.c',
				'pngwrite.c',
				'pngwtran.c',
				'pngwutil.c']

libpng = static_library('libpng', 
						libpng_sources,
						dependencies : m_dep,
						link_whole: zlib,
						include_directories : zlibInclude,
						c_args : ['-DHAVE_CONFIG_H', '-DHAVE_LIBZ'],
						install : true)
