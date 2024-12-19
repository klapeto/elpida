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
target_include_directories(zlibstatic INTERFACE ${ZLIB_INCLUDE_DIRS})
target_include_directories(png INTERFACE ${LIBPNG_INCLUDE_DIRS})
target_include_directories(png_static INTERFACE ${LIBPNG_INCLUDE_DIRS})

add_subdirectory(google-test)

set(BOOST_REGEX_STANDALONE TRUE)
add_subdirectory(boost-regex)

# openssl-cmake cross config
if (MINGW AND CMAKE_HOST_UNIX)
    set(CROSS ON)
    if (CMAKE_SYSTEM_PROCESSOR MATCHES x86_64)
        set(CROSS_TARGET mingw64)
        set(CROSS_PREFIX x86_64-w64-mingw32.static-)
    else()
        set(CROSS_TARGET mingw)
        set(CROSS_PREFIX i686-w64-mingw32.static-)
    endif()
elseif (MINGW)
    if (CMAKE_SYSTEM_PROCESSOR MATCHES AMD64)
        set(OPENSSL_ADDITIONAL_FLAGS mingw64)
    else()
        set(OPENSSL_ADDITIONAL_FLAGS mingw)
    endif()
endif()

if (CMAKE_HOST_UNIX AND CMAKE_CROSSCOMPILING AND CMAKE_SYSTEM_PROCESSOR MATCHES aarch64)
    set(CROSS ON)
    set(CROSS_TARGET linux-aarch64)
    set(CROSS_PREFIX aarch64-linux-gnu-)
endif()

set(BUILD_OPENSSL ON)
set(OPENSSL_USE_STATIC_LIBS ON)
set(OPENSSL_BUILD_VERSION 3.3.1)
set(OPENSSL_BUILD_HASH 777cd596284c883375a2a7a11bf5d2786fc5413255efab20c50d6ffe6d020b7e)
set(OPENSSL_MODULES "no-shared no-asm no-engine no-hw no-cast no-md2 no-md4 no-mdc2 no-rc4 no-rc5 no-engine no-idea no-mdc2 no-rc5 no-camellia no-ssl3 no-heartbeats no-gost no-deprecated no-capieng no-comp no-dtls no-psk no-srp no-dso no-dsa no-rc2 no-des no-apps ${OPENSSL_ADDITIONAL_FLAGS}")
add_subdirectory(openssl-cmake)

# Hack to get the library archive. CMAKE messes up the link order by moving the ws2_32 lib at the beginning
get_property(CRYPTO_INTERFACE_LIB TARGET crypto PROPERTY INTERFACE_LINK_LIBRARIES)
get_property(CRYPTO_LIB TARGET ${CRYPTO_INTERFACE_LIB} PROPERTY IMPORTED_LOCATION)
set(CRYPTO_CUSTOM_LIB_PATH ${CRYPTO_LIB} PARENT_SCOPE)


#set(LLVM_ROOT_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/llvm-project/ CACHE STRING "llvm-project root dir")
#set(LLVM_BUILD_DIR   ${CMAKE_CURRENT_BINARY_DIR}/llvm_build CACHE STRING "llvm-project build dir")
#set(LLVM_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/llvm_install CACHE STRING "llvm-project install dir")
#
#file(MAKE_DIRECTORY ${LLVM_BUILD_DIR})
#ExternalProject_Add(llvm-project
#        PREFIX llvm-project
#        SOURCE_DIR ${LLVM_ROOT_DIR}/llvm
#        BINARY_DIR ${LLVM_BUILD_DIR}
#        #CMAKE_COMMAND x86_64-w64-mingw32.static-cmake
#        CMAKE_ARGS
#        -DCMAKE_BUILD_TYPE=Release
#        -DCMAKE_INSTALL_PREFIX=${LLVM_INSTALL_DIR}
#        -DCMAKE_SKIP_INSTALL_RPATH=ON
#        #            -DBUILD_SHARED_LIBS=OFF
#        #            -DLLVM_STATIC_LINK_CXX_STDLIB=ON
#        -DLLVM_TARGETS_TO_BUILD=X86
#        -DLLVM_BUILD_TOOLS=ON
#        -DLLVM_ENABLE_PROJECTS=clang
#        -DLLVM_BUILD_STATIC=ON
#        -DLLVM_ENABLE_ZLIB=OFF
#        -DLLVM_ENABLE_ZSTD=OFF
#        -DLLVM_INCLUDE_BENCHMARKS=OFF
#        -DLLVM_INCLUDE_EXAMPLES=OFF
#        -DLLVM_INCLUDE_TESTS=OFF
#        -DLIBCLANG_BUILD_STATIC=ON
#)
## Don't always build this, Make target only if the user explicitly calls it
#set_target_properties(llvm-project PROPERTIES EXCLUDE_FROM_ALL TRUE)
#
#set(LLVM_BUILD_ROOT ${LLVM_INSTALL_DIR} PARENT_SCOPE)