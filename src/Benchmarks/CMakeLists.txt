

add_subdirectory(Musl)
link_libraries(-nolibc musl c)

add_subdirectory(Memory)
add_subdirectory(Image)
add_subdirectory(Web)
add_subdirectory(Compression)