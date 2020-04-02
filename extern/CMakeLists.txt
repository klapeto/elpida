#===================================================================================
#  Zlib/Libpng
#  Modified version of the: https://github.com/awstanley/deps-zlib-libpng
#===================================================================================
PROJECT(deps_zlib_libpng C)
# zlib/png integration hack - removes all the errors related to export
# (zlib doesn't export; libpng wants to; in this case we don't care).
SET(SKIP_INSTALL_EXPORT TRUE)

# zlib (requires nothing)
SET(ZLIB_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/zlib")
ADD_SUBDIRECTORY(${ZLIB_DIRECTORY})

GET_DIRECTORY_PROPERTY(
        ZLIB_BUILD_DIRECTORY
        DIRECTORY
        ${ZLIB_DIRECTORY}
        DEFINITION
        CMAKE_CURRENT_BINARY_DIR
)

OPTION(PNG_LINK_ZLIB_STATIC "Use a static zlib library for libpng builds" ON)

# not needed, but paranoia for the future.
GET_DIRECTORY_PROPERTY(
        zlib
        DIRECTORY
        ${ZLIB_DIRECTORY}
        DEFINITION
        zlib
)

# not needed, but paranoia for the future.
GET_DIRECTORY_PROPERTY(
        zlibstatic
        DIRECTORY
        ${ZLIB_DIRECTORY}
        DEFINITION
        zlibstatic
)

IF (PNG_LINK_ZLIB_STATIC)
    SET(ZLIB_LIBRARY zlibstatic)
ELSE ()
    SET(ZLIB_LIBRARY zlib)
ENDIF ()
SET(ZLIB_INCLUDE_DIR ${ZLIB_DIRECTORY})

# libpng (requiring zlib)
SET(LIBPNG_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/libpng")

# libpng needs a hand.
INCLUDE_DIRECTORIES(
        ${ZLIB_DIRECTORY}
        ${ZLIB_BUILD_DIRECTORY}
        ${LIBPNG_DIRECTORY}
        ${LIBPNG_BUILD_DIRECTORY}
)

ADD_SUBDIRECTORY(${LIBPNG_DIRECTORY})

GET_DIRECTORY_PROPERTY(
        LIBPNG_BUILD_DIRECTORY
        DIRECTORY
        ${LIBPNG_DIRECTORY}
        DEFINITION
        CMAKE_CURRENT_BINARY_DIR
)

GET_DIRECTORY_PROPERTY(
        LIBPNG_STATIC
        DIRECTORY
        ${LIBPNG_DIRECTORY}
        DEFINITION
        PNG_LIB_NAME_STATIC
)

GET_DIRECTORY_PROPERTY(
        LIBPNG_SHARED
        DIRECTORY
        ${LIBPNG_DIRECTORY}
        DEFINITION
        PNG_LIB_NAME
)


SET(ZLIB_DIRECTORY ${ZLIB_DIRECTORY} PARENT_SCOPE)
SET(ZLIB_BUILD_DIRECTORY ${ZLIB_BUILD_DIRECTORY} PARENT_SCOPE)
SET(ZLIB_STATIC zlibstatic PARENT_SCOPE)
SET(ZLIB_SHARED zlib PARENT_SCOPE)
SET(LIBPNG_DIRECTORY ${LIBPNG_DIRECTORY} PARENT_SCOPE)
SET(LIBPNG_BUILD_DIRECTORY ${LIBPNG_BUILD_DIRECTORY} PARENT_SCOPE)
SET(LIBPNG_STATIC ${LIBPNG_STATIC} PARENT_SCOPE)
SET(LIBPNG_SHARED ${LIBPNG_SHARED} PARENT_SCOPE)

# Setup the dependencies include directories, sending it upward for use.
IF (NOT DEFINED DEPS_INCLUDE_DIRECTORIES)
    SET(DEPS_INCLUDE_DIRECTORIES
            ${ZLIB_DIRECTORY}
            ${ZLIB_BUILD_DIRECTORY}
            ${LIBPNG_DIRECTORY}
            ${LIBPNG_BUILD_DIRECTORY}
            PARENT_SCOPE
            )
ELSE ()
    # Already defined requires append semantics
    LIST(APPEND DEPS_INCLUDE_DIRECTORIES
            ${ZLIB_DIRECTORY}
            ${ZLIB_BUILD_DIRECTORY}
            ${LIBPNG_DIRECTORY}
            ${LIBPNG_BUILD_DIRECTORY}
            )
ENDIF ()
#===================================================================================
#===================================================================================

#===================================================================================
# hwloc
#===================================================================================

include(ExternalProject)

set(HWLOC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/hwloc)
set(HWLOC_BIN ${CMAKE_CURRENT_BINARY_DIR}/hwloc)
set(HWLOC_STATIC_LIB ${HWLOC_BIN}/lib/libhwloc.so)
set(HWLOC_INCLUDES ${HWLOC_BIN}/include)

file(MAKE_DIRECTORY ${HWLOC_INCLUDES})

ExternalProject_Add(
        libhwloc
        PREFIX ${HWLOC_BIN}
        SOURCE_DIR ${HWLOC_DIR}
        DOWNLOAD_COMMAND cd ${HWLOC_DIR} && git clean -dfX && ${HWLOC_DIR}/autogen.sh
        CONFIGURE_COMMAND ${HWLOC_DIR}/configure --srcdir=${HWLOC_DIR} --prefix=${HWLOC_BIN}
#        CONFIGURE_COMMAND sh -c "${HWLOC_DIR}/configure --srcdir=${HWLOC_DIR} --prefix=${HWLOC_BIN} --enable-static=yes --disable-shared LDFLAGS=-static"
        BUILD_COMMAND make
        INSTALL_COMMAND make install
        BUILD_BYPRODUCTS ${HWLOC_STATIC_LIB}
)

add_library(hwloc SHARED IMPORTED GLOBAL)
add_dependencies(hwloc libhwloc)

message(${HWLOC_STATIC_LIB})
set_target_properties(hwloc PROPERTIES IMPORTED_LOCATION ${HWLOC_STATIC_LIB})
set_target_properties(hwloc PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${HWLOC_INCLUDES})
#===================================================================================
#===================================================================================