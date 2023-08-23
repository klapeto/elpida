cmake_minimum_required(VERSION 3.10)

include(cmake-modules/GetElpidaVersion.cmake)
include(cmake-modules/generate_product_version.cmake)

project(Elpida VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida Simple Benchmarking Framework"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(GenerateProductVersionCurrentDir ${CMAKE_CURRENT_LIST_DIR}/resources)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(ELPIDA_ROOT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake-modules")

message(${CMAKE_MODULE_PATH})

#====================
# RPATH Config
#====================
# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif("${isSystemDir}" STREQUAL "-1")

#====================

include(GNUInstallDirs)

add_compile_options(-fcoroutines)

if (UNIX)
    set(ELPIDA_UNIX ON)
    add_compile_options(-Wextra)
    set(CMAKE_MODULE_LINKER_FLAGS -Wl,--no-undefined)
elseif(MSYS OR MSVC OR MINGW)
    set(ELPIDA_WINDOWS ON)
endif()

if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)
endif()

# Enable debug stuff if we build debug
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(ELPIDA_DEBUG ON)
    set(ELPIDA_DEBUG_BUILD 1)
    add_definitions(-DELPIDA_DEBUG_BUILD=1)
endif()

set(ELPIDA_SHARED_LIBRARY_EXTENSION ${CMAKE_SHARED_LIBRARY_SUFFIX})

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install CACHE PATH "Local" FORCE)
endif()

if (MSYS OR MINGW)
    set(ELPIDA_BENCHMARK_INSTALL_DIR bin/Benchmarks)
    set(ELPIDA_HEADER_INSTALL_DIR include)
    set(ELPIDA_LIB_INSTALL_DIR bin)
    set(ELPIDA_APP_INSTALL_DIR bin)
    add_definitions(-DWIN32=1)
    add_definitions(-DWINVER=0x0600) # We support Vista and above
    add_definitions(-D_WIN32_WINNT=0x0600) # We support Vista and above
else()
    set(ELPIDA_BENCHMARK_INSTALL_DIR bin/Benchmarks)
    set(ELPIDA_HEADER_INSTALL_DIR include)
    set(ELPIDA_LIB_INSTALL_DIR lib)
    set(ELPIDA_APP_INSTALL_DIR bin)
endif()

include_directories(include)

add_subdirectory(extern)
add_subdirectory(src)
add_subdirectory(apps)
