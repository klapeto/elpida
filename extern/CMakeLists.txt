if (ELPIDA_EXTERN_OVERRIDE)
    set(ELPIDA_EXTERN_PATH ${ELPIDA_EXTERN_OVERRIDE})
else ()
    set(ELPIDA_EXTERN_PATH ${CMAKE_CURRENT_SOURCE_DIR})
endif ()

set(ZLIB_BUILD_EXAMPLES OFF)
add_subdirectory(${ELPIDA_EXTERN_PATH}/zlib ${CMAKE_CURRENT_BINARY_DIR}/zlib)

set(ZLIB_INCLUDE_DIRECTORIES
        ${ELPIDA_EXTERN_PATH}/zlib
        ${CMAKE_CURRENT_BINARY_DIR}/zlib
)

set(ZLIB_INCLUDE_DIRECTORIES ${ZLIB_INCLUDE_DIRECTORIES} PARENT_SCOPE)

set(PNG_SHARED OFF)
set(PNG_TESTS OFF)
set(PNG_TOOLS OFF)
# SSE2/NEON/VSX/MSA filter code differs per target (docs/development/code-parity.md)
set(PNG_HARDWARE_OPTIMIZATIONS OFF CACHE BOOL "" FORCE)
set(ZLIB_USE_STATIC_LIBS ON)

if (UNIX)
    set(ZLIB_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/zlib/${CMAKE_STATIC_LIBRARY_PREFIX}z${CMAKE_STATIC_LIBRARY_SUFFIX} CACHE PATH "" FORCE)
else ()
    set(ZLIB_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/zlib/${CMAKE_STATIC_LIBRARY_PREFIX}zlibstatic${CMAKE_STATIC_LIBRARY_SUFFIX} CACHE PATH "" FORCE)
endif ()

set(ZLIB_INCLUDE_DIR ${ZLIB_INCLUDE_DIRECTORIES})
add_subdirectory(${ELPIDA_EXTERN_PATH}/libpng ${CMAKE_CURRENT_BINARY_DIR}/libpng)

set(LIBPNG_INCLUDE_DIRECTORIES
        ${ELPIDA_EXTERN_PATH}/libpng
        ${CMAKE_CURRENT_BINARY_DIR}/libpng
        PARENT_SCOPE
)

set(BOOST_REGEX_STANDALONE ON)
add_subdirectory(${ELPIDA_EXTERN_PATH}/boost-regex ${CMAKE_CURRENT_BINARY_DIR}/boost-regex)

if (WIN32)
    # Detours ships no CMake build, only the core sources are needed for hooking
    set(DETOURS_SOURCE_DIR ${ELPIDA_EXTERN_PATH}/detours/src)
    add_library(detours STATIC
            ${DETOURS_SOURCE_DIR}/detours.cpp
            ${DETOURS_SOURCE_DIR}/disasm.cpp
            ${DETOURS_SOURCE_DIR}/modules.cpp
    )
    target_include_directories(detours PUBLIC ${DETOURS_SOURCE_DIR})
    # -fms-extensions: MSVC integer suffixes (ui64). Detours redefines ERROR_DYNAMIC_CODE_BLOCKED from winerror.h
    target_compile_options(detours PRIVATE -fms-extensions -Wno-macro-redefined -Wno-ignored-attributes)
endif ()

include(../cmake-modules/openssl.cmake)
#include(../cmake-modules/hwloc.cmake)
