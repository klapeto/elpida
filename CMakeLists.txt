cmake_minimum_required(VERSION 3.14)

include(cmake-modules/GetElpidaVersion.cmake)
include(cmake-modules/generate_product_version.cmake)
include(cmake-modules/autotools.env.cmake)

project(Elpida VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida Simple Benchmarking Framework"
        LANGUAGES CXX C)

set(GenerateProductVersionCurrentDir ${CMAKE_CURRENT_LIST_DIR}/resources)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD_REQUIRED True)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(ELPIDA_COMPILER_GCC 1) #most flags work here

    # static link stdlib
    add_link_options(-static-libstdc++ -static-libgcc -Wl,--no-undefined)
    add_compile_options(-fno-builtin)
else ()
    message(FATAL_ERROR "'${CMAKE_CXX_COMPILER_ID}' compiler is currently unsupported. Elpida requires specific compiler functionality that currently only GCC/Clang provide. If you think another compiler can be supported, please let us know in the issues page.")
endif ()

# OS specific flags
if (UNIX)
    set(ELPIDA_UNIX 1)
elseif (MSYS OR MSVC OR MINGW)
    set(ELPIDA_WINDOWS 1)
    add_compile_definitions(-D_UNICODE=1)
    add_compile_definitions(-DUNICODE=1)
#    add_compile_definitions(-DWIN32=1)
#    add_compile_definitions(-DWINVER=0x0600) # We support XP and above
#    add_compile_definitions(-D_WIN32_WINNT=0x0600) # We support XP and above
endif ()

if (LINUX)
    set(ELPIDA_LINUX 1)
endif ()

if (ANDROID)
    set(ELPIDA_ANDROID 1)
endif()

# Enable debug stuff if we build debug
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(ELPIDA_DEBUG 1)
endif ()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/AppDirs/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR} CACHE PATH "Local" FORCE)
endif ()

set(ELPIDA_LOCAL_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install.tmp/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR})
set(ELPIDA_BENCHMARK_INSTALL_DIR bin/Benchmarks)

set(ELPIDA_ROOT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

include_directories(include)

option(ELPIDA_INSTALL_CORE_LIBRARIES "Whether to install the Elpida core libraries on the system" OFF)
option(ELPIDA_INSTALL_COMMON_TASKS_LIBRARIES "Whether to install the Elpida common tasks libraries on the system" OFF)
option(ELPIDA_INSTALL_PREDEFINED_BENCHMARK_LIBRARIES "Whether to install the Elpida predefined benchmark libraries on the system" OFF)

if (ELPIDA_INSTALL_COMMON_TASKS_LIBRARIES)
    set(ELPIDA_INSTALL_CORE_LIBRARIES ON)
endif ()

if (ELPIDA_INSTALL_PREDEFINED_BENCHMARK_LIBRARIES)
    set(ELPIDA_INSTALL_CORE_LIBRARIES ON)
    set(ELPIDA_INSTALL_COMMON_TASKS_LIBRARIES ON)
endif ()

add_subdirectory(extern)
add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(tests)
