cmake_minimum_required(VERSION 3.10)

project(Elpida VERSION 0.0.1
        DESCRIPTION "Simple Benchmarking Framework"
        LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (UNIX)
    set(ELPIDA_LINUX ON)
elseif(MSYS)
    set(ELPIDA_WINDOWS ON)
else()
    message(FATAL_ERROR "Only Linux and Windows (msys) are supported at the moment")
endif()

# Build External Libraries
add_subdirectory(extern)
add_subdirectory(src)
add_subdirectory(apps)

#target_include_directories(Elpida PUBLIC "${PROJECT_SOURCE_DIR}")

#set_target_properties(zlibstatic PROPERTIES COMPILE_FLAGS "-O3 -march=native")
#set_target_properties(zlib PROPERTIES COMPILE_FLAGS "-O3 -march=native")