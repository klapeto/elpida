file(GLOB MUSL_STDLIB_SOURCES *.c)

add_library(musl-stdlib STATIC
        ${MUSL_STDLIB_SOURCES}
)

