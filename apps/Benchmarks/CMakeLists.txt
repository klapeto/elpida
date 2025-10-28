add_executable(compilation-benchmarks Dummy.cpp)
target_link_libraries(compilation-benchmarks PRIVATE elpida-entry-point elpida-compilation-benchmarks)

add_executable(compression-benchmarks Dummy.cpp)
target_link_libraries(compression-benchmarks PRIVATE elpida-entry-point elpida-compression-benchmarks)

add_executable(encryption-benchmarks Dummy.cpp)
target_link_libraries(encryption-benchmarks PRIVATE elpida-entry-point elpida-encryption-benchmarks)

add_executable(image-benchmarks Dummy.cpp)
target_link_libraries(image-benchmarks PRIVATE elpida-entry-point elpida-image-benchmarks)

add_executable(math-benchmarks Dummy.cpp)
target_link_libraries(math-benchmarks PRIVATE elpida-entry-point elpida-math-benchmarks)

add_executable(memory-benchmarks Dummy.cpp)
target_link_libraries(memory-benchmarks PRIVATE elpida-entry-point elpida-memory-benchmarks)

add_executable(stdlib-benchmarks Dummy.cpp)
target_link_libraries(stdlib-benchmarks PRIVATE elpida-entry-point elpida-stdlib-benchmarks)

add_executable(web-benchmarks Dummy.cpp)
target_link_libraries(web-benchmarks PRIVATE elpida-entry-point elpida-web-benchmarks)

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