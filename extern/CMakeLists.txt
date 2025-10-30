set(ZLIB_BUILD_EXAMPLES OFF)
add_subdirectory(zlib)

set(ZLIB_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/zlib
        ${CMAKE_CURRENT_BINARY_DIR}/zlib
)

set(ZLIB_INCLUDE_DIRECTORIES ${ZLIB_INCLUDE_DIRECTORIES} PARENT_SCOPE)

set(PNG_SHARED OFF)
set(PNG_TESTS OFF)
set(PNG_TOOLS OFF)
set(ZLIB_USE_STATIC_LIBS ON)

if (UNIX)
    set(ZLIB_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/zlib/${CMAKE_STATIC_LIBRARY_PREFIX}z${CMAKE_STATIC_LIBRARY_SUFFIX})
else ()
    set(ZLIB_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/zlib/${CMAKE_STATIC_LIBRARY_PREFIX}zlibstatic${CMAKE_STATIC_LIBRARY_SUFFIX})
endif ()

set(ZLIB_INCLUDE_DIR ${ZLIB_INCLUDE_DIRECTORIES})
add_subdirectory(libpng)

set(LIBPNG_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/libpng
        ${CMAKE_CURRENT_BINARY_DIR}/libpng
        PARENT_SCOPE
)

set(BOOST_REGEX_STANDALONE ON)
add_subdirectory(boost-regex)

include(../cmake-modules/openssl.cmake)
include(../cmake-modules/hwloc.cmake)

if (NOT ELPIDA_DISABLE_LLVM)
    include(../cmake-modules/llvm.cmake)
endif ()