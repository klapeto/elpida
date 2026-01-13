add_executable(compilation-benchmarks Dummy.cpp)
target_compile_options(compilation-benchmarks PRIVATE -static)
target_link_libraries(compilation-benchmarks PUBLIC elpida-entry-point elpida-compilation-benchmarks)

add_executable(compression-benchmarks Dummy.cpp)
target_compile_options(compression-benchmarks PRIVATE -static)
target_link_libraries(compression-benchmarks PUBLIC elpida-entry-point elpida-compression-benchmarks)

add_executable(encryption-benchmarks Dummy.cpp)
target_compile_options(encryption-benchmarks PRIVATE -static)
target_link_libraries(encryption-benchmarks PUBLIC elpida-entry-point elpida-encryption-benchmarks)

add_executable(image-benchmarks Dummy.cpp)
target_compile_options(image-benchmarks PRIVATE -static)
target_link_libraries(image-benchmarks PUBLIC elpida-entry-point elpida-image-benchmarks)

add_executable(math-benchmarks Dummy.cpp)
target_compile_options(math-benchmarks PRIVATE -static)
target_link_libraries(math-benchmarks PUBLIC elpida-entry-point elpida-math-benchmarks)

add_executable(memory-benchmarks Dummy.cpp)
target_compile_options(memory-benchmarks PRIVATE -static)
target_link_libraries(memory-benchmarks PUBLIC elpida-entry-point elpida-memory-benchmarks)

add_executable(stdlib-benchmarks Dummy.cpp)
target_compile_options(stdlib-benchmarks PRIVATE -static)
target_link_libraries(stdlib-benchmarks PUBLIC elpida-entry-point elpida-stdlib-benchmarks)

add_executable(web-benchmarks Dummy.cpp)
target_compile_options(web-benchmarks PRIVATE -static)
target_link_libraries(web-benchmarks PUBLIC elpida-entry-point elpida-web-benchmarks)

install(TARGETS
        compilation-benchmarks
        compression-benchmarks
        encryption-benchmarks
        image-benchmarks
        math-benchmarks
        memory-benchmarks
        stdlib-benchmarks
        web-benchmarks
        RUNTIME
        DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})