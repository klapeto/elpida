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

add_subdirectory(google-test)

set(BOOST_REGEX_STANDALONE TRUE)
add_subdirectory(boost-regex)

if (UNIX)
    set(BUILD_OPENSSL ON)
    set(OPENSSL_USE_STATIC_LIBS ON)
    set(OPENSSL_BUILD_VERSION 3.3.1)
    set(OPENSSL_BUILD_HASH 777cd596284c883375a2a7a11bf5d2786fc5413255efab20c50d6ffe6d020b7e)
    add_subdirectory(openssl-cmake)
endif ()