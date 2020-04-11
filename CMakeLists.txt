cmake_minimum_required(VERSION 3.10)

project(Elpida VERSION 0.0.1
        DESCRIPTION "Simple Benchmarking Framework"
        LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(GNUInstallDirs)

if (UNIX)
    set(ELPIDA_LINUX ON)
elseif(MSYS)
    set(ELPIDA_WINDOWS ON)
else()
    message(FATAL_ERROR "Only Linux and Windows (msys) are supported at the moment")
endif()

if (Debug)
    set(ELPIDA_DEBUG ON)
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install CACHE PATH "Local" FORCE)
endif()

if (MSYS)
    set(ELPIDA_TASK_BATCH_INSTALL_DIR bin/TaskBatches)
    set(ELPIDA_HEADER_INSTALL_DIR include)
    set(ELPIDA_LIB_INSTALL_DIR bin)
    set(ELPIDA_APP_INSTALL_DIR bin)
else()
    set(ELPIDA_TASK_BATCH_INSTALL_DIR bin/TaskBatches)
    set(ELPIDA_HEADER_INSTALL_DIR include)
    set(ELPIDA_LIB_INSTALL_DIR lib)
    set(ELPIDA_APP_INSTALL_DIR bin)
endif()

set(CMAKE_MODULE_LINKER_FLAGS -Wl,--no-undefined)

add_subdirectory(extern)
add_subdirectory(src)
add_subdirectory(apps)