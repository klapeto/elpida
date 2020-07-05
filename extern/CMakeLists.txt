#===================================================================================
#  Zlib/Libpng
#  Modified version of the: https://github.com/awstanley/deps-zlib-libpng
#===================================================================================
project(deps_zlib_libpng C)
# zlib/png integration hack - removes all the errors related to export
# (zlib doesn't export; libpng wants to; in this case we don't care).
set(SKIP_INSTALL_EXPORT TRUE)

# zlib (requires nothing)
set(ZLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/zlib")

# Helper
set(ZLIB_CONF_DIR "${CMAKE_CURRENT_BINARY_DIR}/zlib")
set(ZLIB_INCLUDE_DIRS ${ZLIB_DIR} ${ZLIB_CONF_DIR})

include_directories(${ZLIB_INCLUDE_DIRS})

add_subdirectory(${ZLIB_DIR})

get_directory_property(
        ZLIB_BUILD_DIR
        DIRECTORY
        ${ZLIB_DIR}
        DEFINITION
        CMAKE_CURRENT_BINARY_DIR
)

option(PNG_LINK_ZLIB_STATIC "Use a static zlib library for libpng builds" OFF)

# not needed, but paranoia for the future.
get_directory_property(
        zlib
        DIRECTORY
        ${ZLIB_DIR}
        DEFINITION
        zlib
)

# not needed, but paranoia for the future.
get_directory_property(
        zlibstatic
        DIRECTORY
        ${ZLIB_DIR}
        DEFINITION
        zlibstatic
)

if (PNG_LINK_ZLIB_STATIC)
    set(ZLIB_LIBRARY zlibstatic)
else ()
    set(ZLIB_LIBRARY zlib)
endif ()

# Include for png
set(ZLIB_INCLUDE_DIR ${ZLIB_INCLUDE_DIRS})

# libpng (requiring zlib)
set(LIBPNG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libpng")

# libpng needs a hand.
include_directories(
        ${ZLIB_DIR}
        ${ZLIB_BUILD_DIR}
        ${LIBPNG_DIR}
        ${LIBPNG_BUILD_DIR}
)

# we have to force png to use our lib not any system wise
set(PNG_BUILD_ZLIB ON CACHE BOOL "Internal Zlib" FORCE)

add_subdirectory(${LIBPNG_DIR})

get_directory_property(
        LIBPNG_BUILD_DIR
        DIRECTORY
        ${LIBPNG_DIR}
        DEFINITION
        CMAKE_CURRENT_BINARY_DIR
)

get_directory_property(
        LIBPNG_STATIC
        DIRECTORY
        ${LIBPNG_DIR}
        DEFINITION
        PNG_LIB_NAME_STATIC
)

get_directory_property(
        LIBPNG_SHARED
        DIRECTORY
        ${LIBPNG_DIR}
        DEFINITION
        PNG_LIB_NAME
)

set(LIBPNG_INCLUDE_DIRS ${LIBPNG_DIR} ${LIBPNG_BUILD_DIR})

# unless you install them, they do not expose the header includes
target_include_directories(zlib INTERFACE ${ZLIB_INCLUDE_DIRS})
target_include_directories(png INTERFACE ${LIBPNG_INCLUDE_DIRS})

#===================================================================================
#===================================================================================

#===================================================================================
# hwloc
#===================================================================================

include(ExternalProject)

include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
    set(N 1)
endif()

set(HWLOC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/hwloc)

if (DEFINED SNAP_INSTALL_PREFIX)
    set(HWLOC_BIN ${SNAP_INSTALL})
else()
    set(HWLOC_BIN ${CMAKE_INSTALL_PREFIX})
endif()

if (UNIX)
    set(HWLOC_LIB ${HWLOC_BIN}/lib/libhwloc.so)
else ()
    set(HWLOC_LIB ${HWLOC_BIN}/bin/libhwloc-15.dll)
endif ()

set(HWLOC_IMPLIB ${HWLOC_BIN}/lib/libhwloc${CMAKE_IMPORT_LIBRARY_SUFFIX})

set(HWLOC_INCLUDES ${HWLOC_BIN}/include)

file(MAKE_DIRECTORY ${HWLOC_INCLUDES})

ExternalProject_Add(
        libhwloc
        PREFIX ${HWLOC_BIN}
        SOURCE_DIR ${HWLOC_DIR}
        DOWNLOAD_COMMAND cd ${HWLOC_DIR} && git clean -dfX && ${HWLOC_DIR}/autogen.sh
        CONFIGURE_COMMAND ${HWLOC_DIR}/configure --srcdir=${HWLOC_DIR} --prefix=${HWLOC_BIN}
        #        CONFIGURE_COMMAND sh -c "${HWLOC_DIR}/configure --srcdir=${HWLOC_DIR} --prefix=${HWLOC_BIN} --enable-static=yes --disable-shared LDFLAGS=-static"
        BUILD_COMMAND make install -j${N}
        INSTALL_COMMAND make install
        BUILD_BYPRODUCTS ${HWLOC_LIB} ${HWLOC_IMPLIB}
)

add_library(hwloc SHARED IMPORTED GLOBAL)
add_dependencies(hwloc libhwloc)

set_target_properties(hwloc PROPERTIES
        IMPORTED_LOCATION ${HWLOC_LIB}
        IMPORTED_IMPLIB ${HWLOC_IMPLIB}
        INTERFACE_INCLUDE_DIRECTORIES ${HWLOC_INCLUDES}
        )
#===================================================================================
#===================================================================================