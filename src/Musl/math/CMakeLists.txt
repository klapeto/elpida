file(GLOB MUSL_MATH_SOURCES *.c)

if (${ARCH} STREQUAL "x86_64")
    file(GLOB MUSL_MATH_X86_64_SOURCES x86_64/*.*)
endif ()

if (${ARCH} STREQUAL "aarch64")
    file(GLOB MUSL_MATH_AARCH64_SOURCES aarch64/*.*)
endif ()

add_library(musl-math STATIC
        ${MUSL_MATH_X86_64_SOURCES}
        ${MUSL_MATH_AARCH64_SOURCES}
        ${MUSL_MATH_SOURCES}
)

