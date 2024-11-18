project(musl-lite LANGUAGES C)

add_library(musl-lite
        string/memccpy.c
        string/memchr.c
        string/memcmp.c
        string/memcpy.c
        string/memmem.c
        string/memmove.c
        string/mempcpy.c
        string/memrchr.c
        string/memset.c
)

target_compile_options(musl-lite PRIVATE -ffreestanding -O3)