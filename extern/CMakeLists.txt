add_subdirectory(zlib)

set(PNG_BUILD_ZLIB ON CACHE BOOL "Internal Zlib" FORCE)
set(ZLIB_LIBRARY zlibstatic)
set(ZLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/zlib")
set(ZLIB_CONF_DIR "${CMAKE_CURRENT_BINARY_DIR}/zlib")
set(ZLIB_INCLUDE_DIRS ${ZLIB_DIR} ${ZLIB_CONF_DIR})

include_directories(${ZLIB_INCLUDE_DIRS})
set(ZLIB_INCLUDE_DIR ${ZLIB_INCLUDE_DIRS})

set(SKIP_INSTALL_EXPORT TRUE)
add_subdirectory(libpng)

set(INSTALL_GTEST CACHE BOOL FALSE FORCE)
add_subdirectory(google-test)
