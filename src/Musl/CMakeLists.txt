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

file(GLOB MUSL_SOURCES src/internal/*.c src/math/*.c src/stdio/*.c src/stdlib/*.c src/string/*.c src/thread/*.c)
file(GLOB MUSL_MATH_ARCH_SOURCES src/math/${ARCH}/*.c src/math/${ARCH}/*.s src/string/${ARCH}/*.c src/string/${ARCH}/*.s)

add_library(musl STATIC ${MUSL_MATH_ARCH_SOURCES} ${MUSL_SOURCES})

execute_process(COMMAND sed -f ${CMAKE_CURRENT_LIST_DIR}/tools/mkalltypes.sed ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH}/bits/alltypes.h.in ${CMAKE_CURRENT_LIST_DIR}/include/alltypes.h.in OUTPUT_FILE ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH}/bits/alltypes.h)
execute_process(COMMAND sed -n -e s/__NR_/SYS_/p INPUT_FILE ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH}/bits/syscall.h.in OUTPUT_FILE ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH}/bits/syscall.h)

target_compile_options(musl PRIVATE -nostartfiles -nolibc -nostdinc -ffreestanding)

target_include_directories(musl PRIVATE ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH})
target_include_directories(musl PRIVATE ${CMAKE_CURRENT_LIST_DIR}/arch/generic)
target_include_directories(musl PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/include)
target_include_directories(musl PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/internal)
target_include_directories(musl PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)