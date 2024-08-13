if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set(ARCH "x86_64")
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    set(ARCH "aarch64")
else()
    set(ARCH "generic")
endif()

add_subdirectory(math)
add_subdirectory(string)
add_subdirectory(stdlib)

add_library(musl STATIC dummy.c)

target_link_libraries(musl musl-math musl-string musl-stdlib)

target_compile_options(musl PRIVATE -nostartfiles)