file(GLOB MUSL_STRING_SOURCES *.c)

if (${ARCH} STREQUAL "x86_64")
    file(GLOB MUSL_STRING_X86_64_SOURCES x86_64/*.*)
endif ()

if (${ARCH} STREQUAL "i386" OR ${ARCH} STREQUAL "i686")
    file(GLOB MUSL_MATH_I368_SOURCES i386/*.*)
endif ()

if (${ARCH} STREQUAL "aarch64")
    file(GLOB MUSL_MATH_AARCH64_SOURCES i386/*.*)
endif ()

add_library(musl-string STATIC
        ${MUSL_STRING_X86_64_SOURCES}
        ${MUSL_MATH_I368_SOURCES}
        ${MUSL_MATH_AARCH64_SOURCES}
        ${MUSL_STRING_SOURCES}
)

