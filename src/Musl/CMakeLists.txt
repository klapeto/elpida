project("musl"
        DESCRIPTION "musl"
        LANGUAGES C ASM)

if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set(ARCH "x86_64")
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    set(ARCH "aarch64")
else()
    set(ARCH "generic")
endif()

set(MUSL_SOURCES
        src/internal/floatscan.c
        src/internal/shgetc.c
        src/stdio/__lockfile.c
        src/stdio/__stdio_exit.c
        src/stdio/__toread.c
        src/stdio/__uflow.c
        src/stdio/ofl.c
        src/stdlib/strtod.c
        src/thread/__lock.c
        src/internal/libc.c
)

add_library(musl STATIC ${MUSL_SOURCES})

execute_process(COMMAND sed -f ${CMAKE_CURRENT_LIST_DIR}/tools/mkalltypes.sed ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH}/bits/alltypes.h.in ${CMAKE_CURRENT_LIST_DIR}/include/alltypes.h.in OUTPUT_FILE ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH}/bits/alltypes.h)
execute_process(COMMAND sed -n -e s/__NR_/SYS_/p INPUT_FILE ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH}/bits/syscall.h.in OUTPUT_FILE ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH}/bits/syscall.h)

target_compile_options(musl PRIVATE -nostartfiles -nolibc -nostdinc -ffreestanding)

target_include_directories(musl PRIVATE ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH})
target_include_directories(musl PRIVATE ${CMAKE_CURRENT_LIST_DIR}/arch/generic)
target_include_directories(musl PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/include)
target_include_directories(musl PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/internal)
target_include_directories(musl PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)