cmake_minimum_required(VERSION 3.22.1)

project("Elpida")

set(ELPIDA_ENABLE_CLI ON)

add_link_options(
        "-Wl,-z,max-page-size=16384"
        "-Wl,-z,common-page-size=16384"
)
link_directories(
        ${CMAKE_SYSROOT}/usr/lib/${CMAKE_SYSTEM_PROCESSOR}-linux-android
)

add_subdirectory(../../../../../../.. ${CMAKE_CURRENT_BINARY_DIR}/elpida)

include_directories(Elpida PRIVATE ../../../../../../../include ../../../../../../../apps/Application)

add_library(Elpida SHARED Elpida.cpp)
target_link_libraries(Elpida
        android
        elpida-core
        elpida-application
        elpida-overheads-calculator
        elpida-platform
        elpida-shared
        log)