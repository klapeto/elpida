add_library(musl-lite OBJECT
        string/memccpy.c
        string/memchr.c
        string/memcmp.c
        string/memcpy.c
        string/memmem.c
        string/memmove.c
        string/mempcpy.c
        string/memrchr.c
        string/memset.c
        string/strchrnul.c
        string/strlen.c
        string/strchr.c
)

target_compile_options(musl-lite PRIVATE -ffreestanding)