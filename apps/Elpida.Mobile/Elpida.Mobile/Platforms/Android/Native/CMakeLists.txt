cmake_minimum_required(VERSION 3.22.1)

project("Elpida")

# Android runs the same benchmark executables as the desktop apps (docs/development/android.md). Everything is built
# with cmake-modules/llvm-cross.cmake and the Android targets of the llvm-mingw toolchain, in two separate builds:
#
#  ELPIDA_ANDROID_PART=benchmarks  (TARGET_PREFIX=<triple>29-)
#     The benchmark executables. They must run the same code as on every other platform, so they are linked
#     statically: they then use ELF TLS and mimalloc's __thread model even on Android older than 10, and do not
#     depend on the device's libc.so.
#
#  ELPIDA_ANDROID_PART=app  (TARGET_PREFIX=<triple>21-, the app's minSdk)
#     libElpida.so, which the .NET app P/Invokes, and elpida-info-dumper. They are not benchmark code, so they are
#     ordinary dynamically linked binaries the device's own loader accepts.
#     Also stdlib-benchmarks: it measures the device's own C library and allocator (bionic's libc.so), so it is
#     dynamically linked against it, without musl-lite and mimalloc (docs/development/code-parity.md).
#
# Executables are packaged as native libraries (lib<name>.so) so that Android extracts them to the native library
# directory, the only app location they are allowed to execute from.

set(ELPIDA_ANDROID_PART "" CACHE STRING "The part to build: benchmarks or app")
set_property(CACHE ELPIDA_ANDROID_PART PROPERTY STRINGS benchmarks app)

if (ELPIDA_ANDROID_PART STREQUAL "benchmarks")
    set(ELPIDA_ENABLE_BENCHMARKS ON CACHE BOOL "" FORCE)
    set(ELPIDA_ENABLE_STDLIB_BENCHMARKS OFF CACHE BOOL "" FORCE)
elseif (ELPIDA_ANDROID_PART STREQUAL "app")
    set(ELPIDA_ENABLE_BENCHMARKS OFF CACHE BOOL "" FORCE)
    set(ELPIDA_ENABLE_STDLIB_BENCHMARKS ON CACHE BOOL "" FORCE)
else ()
    message(FATAL_ERROR "Set ELPIDA_ANDROID_PART to 'benchmarks' or 'app'")
endif ()

set(ELPIDA_ENABLE_BENCHMARK_TOOLS ON)

add_link_options(
        "-Wl,-z,max-page-size=16384"
        "-Wl,-z,common-page-size=16384"
)

add_subdirectory(../../../../../.. ${CMAKE_CURRENT_BINARY_DIR}/elpida)

function(elpida_android_executable executable)
    set_target_properties(${executable} PROPERTIES
            PREFIX "lib"
            SUFFIX ".so"
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endfunction()

if (ELPIDA_ANDROID_PART STREQUAL "benchmarks")
    set(ELPIDA_ANDROID_BENCHMARKS
            compilation-benchmarks
            compression-benchmarks
            encryption-benchmarks
            image-benchmarks
            math-benchmarks
            memory-benchmarks
            web-benchmarks
    )

    add_custom_target(elpida-android-benchmarks ALL)
    foreach (executable ${ELPIDA_ANDROID_BENCHMARKS})
        elpida_android_executable(${executable})
        target_link_options(${executable} PRIVATE -static)
        add_dependencies(elpida-android-benchmarks ${executable})
    endforeach ()
else ()
    include_directories(Elpida PRIVATE ../../../../../../include ../../../../../../apps/Application)

    # Only what libElpida.so links is built as PIC (the NDK toolchain file used to enable PIC globally)
    set_target_properties(elpida-core elpida-application elpida-platform elpida-shared elpida-jep106 PROPERTIES
            POSITION_INDEPENDENT_CODE ON)

    # The library the .NET app P/Invokes. It only drives the executables and must not link mimalloc.
    add_library(Elpida SHARED Elpida.cpp)

    target_link_libraries(Elpida
            android
            elpida-core
            elpida-application
            elpida-platform
            elpida-shared
            log)

    elpida_android_executable(elpida-info-dumper)
    add_dependencies(Elpida elpida-info-dumper)

    # Dynamically linked against the device's libc.so (see above)
    elpida_android_executable(stdlib-benchmarks)
    add_dependencies(Elpida stdlib-benchmarks)
endif ()
