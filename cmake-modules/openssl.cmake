# MIT License
#
# Copyright (c) 2015-2024 The ViaDuck Project
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

# heavily modified code from The ViaDuck Project

include(ProcessorCount)
ProcessorCount(NUM_JOBS)
include(ExternalProject)

set(OPENSSL_BUILD_VERSION 3.3.1)
set(OPENSSL_BUILD_HASH 777cd596284c883375a2a7a11bf5d2786fc5413255efab20c50d6ffe6d020b7e)
set(OPENSSL_MODULES "no-shared no-asm no-engine no-hw no-cast no-md2 no-md4 no-mdc2 no-rc4 no-rc5 no-engine no-idea no-mdc2 no-rc5 no-camellia no-ssl3 no-heartbeats no-gost no-deprecated no-capieng no-comp no-dtls no-psk no-srp no-dso no-dsa no-rc2 no-des no-apps")
set(OPENSSL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/openssl-install)

if (CMAKE_CROSSCOMPILING)
    if (CMAKE_SYSTEM_NAME STREQUAL Linux AND CMAKE_SYSTEM_PROCESSOR MATCHES aarch64)
        set(OPENSSL_ADDITIONAL_FLAGS "${OPENSSL_ADDITIONAL_FLAGS} linux-aarch64")
    elseif (CMAKE_SYSTEM_NAME STREQUAL Windows AND CMAKE_SYSTEM_PROCESSOR MATCHES x86_64)
        set(OPENSSL_ADDITIONAL_FLAGS "${OPENSSL_ADDITIONAL_FLAGS} mingw64")
    elseif (CMAKE_SYSTEM_NAME STREQUAL Windows AND CMAKE_SYSTEM_PROCESSOR MATCHES i686)
        set(OPENSSL_ADDITIONAL_FLAGS "${OPENSSL_ADDITIONAL_FLAGS} mingw")
    endif()
endif ()

set(OPENSSL_INCLUDE_DIR "${OPENSSL_PREFIX}/include")
file(MAKE_DIRECTORY ${OPENSSL_INCLUDE_DIR})
file(MAKE_DIRECTORY ${OPENSSL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

foreach(OPENSSL_BASE_NAME crypto ssl)
    set(OPENSSL_STATIC_LIB ${OPENSSL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${OPENSSL_BASE_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX})

    add_library(${OPENSSL_BASE_NAME} STATIC IMPORTED GLOBAL)
    set_property(TARGET ${OPENSSL_BASE_NAME} PROPERTY IMPORTED_LOCATION ${OPENSSL_STATIC_LIB})

    list(APPEND ${OPENSSL_BYPRODUCTS_VAR} ${OPENSSL_STATIC_LIB})
endforeach()

set(OPENSSL_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/src)
set(OPENSSL_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/openssl.config.sh)
generate_environment_file(${OPENSSL_CONFIG_FILE} "${OPENSSL_SRC_DIR}/Configure --prefix=${OPENSSL_PREFIX} --libdir=${CMAKE_INSTALL_LIBDIR} --openssldir=${OPENSSL_PREFIX} ${OPENSSL_MODULES} ${OPENSSL_ADDITIONAL_FLAGS}")

find_program(MAKE_PROGRAM make)

ExternalProject_Add(openssl
        URL https://mirror.viaduck.org/openssl/openssl-${OPENSSL_BUILD_VERSION}.tar.gz
        ${OPENSSL_CHECK_HASH}
        SOURCE_DIR ${OPENSSL_SRC_DIR}
        CONFIGURE_COMMAND ${OPENSSL_CONFIG_FILE}

        BUILD_COMMAND ${MAKE_PROGRAM} -j ${NUM_JOBS}
        BUILD_BYPRODUCTS ${OPENSSL_BYPRODUCTS}

        INSTALL_COMMAND ${MAKE_PROGRAM} install_sw
)

add_dependencies(ssl openssl)
add_dependencies(crypto openssl)

# set include locations
target_include_directories(ssl BEFORE INTERFACE ${OPENSSL_INCLUDE_DIR})
target_include_directories(crypto BEFORE INTERFACE ${OPENSSL_INCLUDE_DIR})

