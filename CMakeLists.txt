cmake_minimum_required(VERSION 3.14)

include(cmake-modules/GetElpidaVersion.cmake)
include(cmake-modules/generate_product_version.cmake)
include(cmake-modules/autotools.env.cmake)

# force compilers for testing purposes only
#set(CMAKE_C_COMPILER /usr/bin/gcc-9 CACHE PATH "" FORCE)
#set(CMAKE_CXX_COMPILER /usr/bin/g++-9 CACHE PATH "" FORCE)

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

    # ubuntu does not ship static libs for GL/EGL to be able to static link...
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-builtin -static")
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-builtin -static")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-builtin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-builtin")

    # Benchmark code parity (docs/development/code-parity.md): make the generated code independent of per-target
    # compiler defaults. Keep in sync with scripts/docker/parity-flags.sh (toolchain runtimes and LLVM libraries).
    # -fno-math-errno: clang defaults differ (on for glibc/mingw, off for Android/musl). musl never sets errno in
    #                  math functions, and with this sqrt() etc. map to the ISA's instruction on every target.
    # -ffp-contract=off: do not fuse a*b+c into FMA only on the targets that have it.
    # -fomit-frame-pointer: clang keeps frame pointers by default on Android (all ABIs) and aarch64 Linux only.
    set(ELPIDA_PARITY_FLAGS "-fno-math-errno -ffp-contract=off -fomit-frame-pointer")
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64|ARM64)$")
        # Outline atomics pick LSE or LL/SC at runtime depending on the CPU.
        # x18 is reserved on Android and Windows (platform register) but allocatable on Linux: reserve it everywhere.
        # Cortex-A53 errata: clang enables the 835769 workaround (a nop between a memory access and a 64-bit
        # multiply-accumulate) and the linker's 843419 fix by default for Android only. Enable them everywhere: they
        # are needed on real A53 cores (Raspberry Pi 3, older phones) anyway. lld has the 843419 fix for ELF only.
        set(ELPIDA_PARITY_FLAGS "${ELPIDA_PARITY_FLAGS} -mno-outline-atomics -ffixed-x18 -mfix-cortex-a53-835769")
        if (NOT WIN32)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--fix-cortex-a53-843419")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--fix-cortex-a53-843419")
        endif ()
    elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64)$")
        # Fixed baseline: the Android triple otherwise enables SSE4.2/POPCNT/CX16 by default
        set(ELPIDA_PARITY_FLAGS "${ELPIDA_PARITY_FLAGS} -march=x86-64 -mtune=generic -mno-sse3 -mno-ssse3 -mno-sse4.1 -mno-sse4.2 -mno-popcnt -mno-cx16")
    elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(i386|i586|i686)$")
        # Fixed baseline (llvm-mingw default). The Android triple otherwise uses i686+SSSE3
        set(ELPIDA_PARITY_FLAGS "${ELPIDA_PARITY_FLAGS} -march=pentium4 -mtune=generic -mno-sse3 -mno-ssse3")
    endif ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ELPIDA_PARITY_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ELPIDA_PARITY_FLAGS}")

    # if we link with musl, allow this
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--allow-multiple-definition")

    # Binary size: drop unreachable functions/data at link time (mostly LLVM/Clang code in the Compilation benchmark,
    # whose libraries are already built with function sections). Only unreachable code is removed, so the executed
    # code does not change. lld accepts --gc-sections for ELF and MinGW (/OPT:REF).
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--gc-sections")
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
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g3 -gcodeview)
        add_link_options(-Wl,--pdb=)
    endif ()
    add_link_options(-Wl,--major-subsystem-version,5 -Wl,--minor-subsystem-version,1 -lpsapi -lntdll) # for windows XP support
#    add_compile_definitions(-DWIN32=1)
#    add_compile_definitions(-DWINVER=0x0600) # We support XP and above
#    add_compile_definitions(-D_WIN32_WINNT=0x0600) # We support XP and above
endif ()

if (LINUX AND NOT ANDROID) # Android builds with llvm-cross.cmake are LINUX too
    set(ELPIDA_LINUX 1)
    # No -lgcc: it came before compiler-rt on the link line, so lld took the compiler builtins (e.g. __mulsc3 in the FFT
    # benchmark) from the sysroot's libgcc instead of compiler-rt like every other platform (docs/development/code-parity.md)

    # Link libc++/libc++abi (-static-libstdc++) and libunwind (-static-libgcc) statically, like Windows (-static), so
    # no executable depends on the toolchain's libc++.so/libunwind.so (and whatever they pull in). A fully static build
    # is not possible: Qt loads GL/EGL/xcb with dlopen. Qt itself is built static, so no shared library uses libc++.
    add_link_options(-static-libstdc++ -static-libgcc)
endif ()

if (ANDROID)
    set(ELPIDA_ANDROID 1)
    # The NDK toolchain file defines it and some code checks it (httplib); no benchmark code does
    add_compile_definitions(ANDROID)
endif()

# Enable debug stuff if we build debug
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(ELPIDA_DEBUG 1)
endif ()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND ELPIDA_DEBUG)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install CACHE PATH "Local" FORCE)
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

# Packages are made from the bin directory, so the licenses go along with the binaries
install(FILES LICENSE src/THIRD_PARTY_NOTICES.txt DESTINATION bin)