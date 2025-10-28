set(ZLIB_BUILD_EXAMPLES OFF)
add_subdirectory(zlib)

set(ZLIB_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/zlib
        ${CMAKE_CURRENT_BINARY_DIR}/zlib
        PARENT_SCOPE
)

set(PNG_SHARED OFF)
set(PNG_TESTS OFF)
set(PNG_TOOLS OFF)
set(ZLIB_USE_STATIC_LIBS ON)
set(ZLIB_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/zlib/libz.a)
set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/zlib)
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
include(../cmake-modules/llvm.cmake)