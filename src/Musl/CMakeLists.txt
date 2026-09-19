if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
    set(ARCH "x86_64")
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "i686" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "i386")
    set(ARCH "i386")
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64ec" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "ARM64")
    set(ARCH "aarch64")
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "armv7" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "arm" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "armv7-a")
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

add_library(musl-lite OBJECT ${MUSL_SOURCES})

find_program(SED_EXECUTABLE NAMES sed REQUIRED)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/arch/${ARCH}/bits)

execute_process(COMMAND ${SED_EXECUTABLE} -f ${CMAKE_CURRENT_LIST_DIR}/tools/mkalltypes.sed ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH}/bits/alltypes.h.in ${CMAKE_CURRENT_LIST_DIR}/include/alltypes.h.in OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/arch/${ARCH}/bits/alltypes.h)
#execute_process(COMMAND sed -n -e s/__NR_/SYS_/p INPUT_FILE ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH}/bits/syscall.h.in OUTPUT_FILE ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH}/bits/syscall.h)

target_compile_options(musl-lite PRIVATE -nostdinc -ffreestanding)

target_include_directories(musl-lite PRIVATE ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH})
target_include_directories(musl-lite PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/arch/${ARCH})
target_include_directories(musl-lite PRIVATE ${CMAKE_CURRENT_LIST_DIR}/arch/generic)
target_include_directories(musl-lite PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/include)
target_include_directories(musl-lite PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/internal)
target_include_directories(musl-lite PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

block()

    # Check whether compiler's long double definition matches float.h
    include(CheckCSourceCompiles)
    unset(CMAKE_C_STANDARD)
    unset(CMAKE_SYSROOT)
    set(CMAKE_C_STANDARD 99)
    set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
    set(CMAKE_REQUIRED_FLAGS "-std=c99 -nostdinc -ffreestanding -fno-builtin -fexcess-precision=standard -frounding-math -fno-strict-aliasing -Wa,--noexecstack")

    # for aarch64-w64-mingw32 long double is 64bit
    if (ELPIDA_WINDOWS)
        set(CMAKE_REQUIRED_FLAGS "-DELPIDA_WINDOWS ${CMAKE_REQUIRED_FLAGS}")
    endif ()
    if (ELPIDA_ANDROID)
        set(CMAKE_REQUIRED_FLAGS "-DELPIDA_ANDROID ${CMAKE_REQUIRED_FLAGS}")
    endif ()

    set(CMAKE_REQUIRED_INCLUDES "${CMAKE_CURRENT_BINARY_DIR}/arch/${ARCH};${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH};${CMAKE_CURRENT_LIST_DIR}/arch/generic;${CMAKE_CURRENT_LIST_DIR}/include")

    check_c_source_compiles("
        #include <float.h>
        #define C(m,s) (m==LDBL_MANT_DIG && s==sizeof(long double))
        typedef char ldcheck[(C(53,8)||C(64,12)||C(64,16)||C(113,16))*2-1];
    " LONG_DOUBLE_MATCHES_FLOAT_H)

    check_c_source_compiles([[
        typedef int x;
        #if __FAST_MATH__
        #error yes
        #endif
    ]] PASSED_FAST_MATH)

    check_c_source_compiles([[typedef char dblcheck[(int)sizeof(double)-5];]] IEEE_DOUBLE_CORRECT)

    check_c_source_compiles([[
        float f(float x) { __asm__(\"\":\"+t\"(x)); return x; }
    ]] WORKING_CLANG_FLOAT_CONSTRAINTS)

    if (NOT PASSED_FAST_MATH)
        message(FATAL_ERROR "__FAST_MATH__ was passed to the compiler. Remove it from musl")
    endif ()

    if (NOT IEEE_DOUBLE_CORRECT)
        message(FATAL_ERROR "compiler's floating point configuration is unsupported")
    endif ()

    if (NOT LONG_DOUBLE_MATCHES_FLOAT_H)
        message(FATAL_ERROR "unsupported long double type")
    endif ()

    if (NOT WORKING_CLANG_FLOAT_CONSTRAINTS)
        target_compile_definitions(musl-lite PRIVATE -DBROKEN_VFP_ASM)
    endif ()
endblock()

if (ANDROID AND ARCH STREQUAL "x86_64")
    target_compile_options(musl-lite PRIVATE -mlong-double-80)
endif ()

target_link_libraries(musl-lite PUBLIC musl-platform)

if (ELPIDA_INSTALL_CORE_LIBRARIES)
    install(TARGETS musl-lite ARCHIVE)
endif ()