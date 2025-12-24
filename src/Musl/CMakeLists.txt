if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
    set(ARCH "x86_64")
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "i686")
    set(ARCH "i386")
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64ec" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "ARM64")
    set(ARCH "aarch64")
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "armv7" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "arm")
    set(ARCH "arm")
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "riscv64")
    set(ARCH "riscv64")
else ()
    set(ARCH "generic")
endif ()

add_library(musl-platform
        platform/errno-location.c
)

file(GLOB MUSL_SOURCES src/internal/*.c src/math/*.c src/stdio/*.c src/stdlib/*.c src/string/*.c)

add_library(musl-lite ${MUSL_SOURCES})

find_program(SED_EXECUTABLE NAMES sed REQUIRED)

execute_process(COMMAND ${SED_EXECUTABLE} -f ${CMAKE_CURRENT_LIST_DIR}/tools/mkalltypes.sed ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH}/bits/alltypes.h.in ${CMAKE_CURRENT_LIST_DIR}/include/alltypes.h.in OUTPUT_FILE ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH}/bits/alltypes.h)
#execute_process(COMMAND sed -n -e s/__NR_/SYS_/p INPUT_FILE ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH}/bits/syscall.h.in OUTPUT_FILE ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH}/bits/syscall.h)

target_compile_options(musl-lite PRIVATE -nostdinc -ffreestanding)

target_include_directories(musl-lite PRIVATE ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH})
target_include_directories(musl-lite PRIVATE ${CMAKE_CURRENT_LIST_DIR}/arch/generic)
target_include_directories(musl-lite PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/include)
target_include_directories(musl-lite PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/internal)
target_include_directories(musl-lite PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(musl-lite PUBLIC musl-platform)

if (ELPIDA_INSTALL_CORE_LIBRARIES)
    install(TARGETS musl-lite ARCHIVE)
endif ()