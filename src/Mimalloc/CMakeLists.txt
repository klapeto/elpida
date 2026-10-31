project("mimalloc")

# mimalloc replaces the process allocator. Not on Android: a dlopen'ed library cannot replace bionic's allocator
# for the process, and mixing both allocators across module boundaries crashes.
if (ELPIDA_ANDROID)
    add_library(mimalloc INTERFACE)
    return()
endif ()

add_library(mimalloc OBJECT static.c)
target_include_directories(mimalloc PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
if (WIN32)
    target_link_libraries(mimalloc PUBLIC minhook)
endif ()
