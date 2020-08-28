cmake_minimum_required(VERSION 3.10)

project(Elpida VERSION 0.0.1
        DESCRIPTION "Simple Benchmarking Framework"
        LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake-modules")

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

if (UNIX)
    set(ELPIDA_LINUX ON)
    add_compile_options(-Wextra)
    set(CMAKE_MODULE_LINKER_FLAGS -Wl,--no-undefined)
elseif(MSYS OR MSVC)
    set(ELPIDA_WINDOWS ON)
endif()

if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(ELPIDA_DEBUG ON)
    set(ELPIDA_DEBUG_BUILD 1)
    add_definitions(-DELPIDA_DEBUG_BUILD=1)
endif()

set(ELPIDA_SHARED_LIBRARY_EXTENSION ${CMAKE_SHARED_LIBRARY_SUFFIX})

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install CACHE PATH "Local" FORCE)
endif()

if (MSYS)
    set(ELPIDA_TASK_BATCH_INSTALL_DIR bin/Benchmarks)
    set(ELPIDA_HEADER_INSTALL_DIR include)
    set(ELPIDA_LIB_INSTALL_DIR bin)
    set(ELPIDA_APP_INSTALL_DIR bin)
else()
    set(ELPIDA_TASK_BATCH_INSTALL_DIR bin/Benchmarks)
    set(ELPIDA_HEADER_INSTALL_DIR include)
    set(ELPIDA_LIB_INSTALL_DIR lib)
    set(ELPIDA_APP_INSTALL_DIR bin)
endif()

add_subdirectory(extern)
add_subdirectory(src)
add_subdirectory(apps)
