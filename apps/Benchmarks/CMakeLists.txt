function(GenerateSources BenchmarkName BenchmarkDescription outfiles)
    set(VersionFiles "")
    if (MSYS OR MINGW OR MSVC)
        generate_product_version(
                VersionFiles
                NAME ${BenchmarkName}
                ICON ${ELPIDA_ROOT_DIRECTORY}/images/Elpida-icon.ico
                VERSION_MAJOR ${ELPIDA_BENCHMARKS_VERSION_MAJOR}
                VERSION_MINOR ${ELPIDA_BENCHMARKS_VERSION_MINOR}
                VERSION_PATCH ${ELPIDA_BENCHMARKS_VERSION_REVISION}
                VERSION_REVISION ${ELPIDA_BENCHMARKS_VERSION_BUILD}
                COMPANY_NAME ${ELPIDA_COMPANY_NAME}
                FILE_DESCRIPTION ${BenchmarkDescription}
        )
    endif()

    list(APPEND ${outfiles} ${VersionFiles} Dummy.cpp)
    set(${outfiles} ${${outfiles}} PARENT_SCOPE)
endfunction()

GenerateSources("Elpida Compilation Benchmarks" "Performs compilation benchmarks and outputs the results" COMPILATION_BENCHMARKS_SOURCES)
add_executable(compilation-benchmarks ${COMPILATION_BENCHMARKS_SOURCES})
target_link_libraries(compilation-benchmarks PUBLIC elpida-entry-point elpida-compilation-benchmarks)

GenerateSources("Elpida Compression Benchmarks" "Performs compression benchmarks and outputs the results" COMPRESSION_BENCHMARKS_SOURCES)
add_executable(compression-benchmarks ${COMPRESSION_BENCHMARKS_SOURCES})
target_link_libraries(compression-benchmarks PUBLIC elpida-entry-point elpida-compression-benchmarks)

GenerateSources("Elpida Encryption Benchmarks" "Performs encryption benchmarks and outputs the results" ENCRYPTION_BENCHMARKS_SOURCES)
add_executable(encryption-benchmarks ${ENCRYPTION_BENCHMARKS_SOURCES})
target_link_libraries(encryption-benchmarks PUBLIC elpida-entry-point elpida-encryption-benchmarks)

GenerateSources("Elpida Image Benchmarks" "Performs image benchmarks and outputs the results" IMAGE_BENCHMARKS_SOURCES)
add_executable(image-benchmarks ${IMAGE_BENCHMARKS_SOURCES})
target_link_libraries(image-benchmarks PUBLIC elpida-entry-point elpida-image-benchmarks)

GenerateSources("Elpida Math Benchmarks" "Performs math benchmarks and outputs the results" MATH_BENCHMARKS_SOURCES)
add_executable(math-benchmarks ${MATH_BENCHMARKS_SOURCES})
target_link_libraries(math-benchmarks PUBLIC elpida-entry-point elpida-math-benchmarks)

GenerateSources("Elpida Memory Benchmarks" "Performs memory benchmarks and outputs the results" MEMORY_BENCHMARKS_SOURCES)
add_executable(memory-benchmarks ${MEMORY_BENCHMARKS_SOURCES})
target_link_libraries(memory-benchmarks PUBLIC elpida-entry-point elpida-memory-benchmarks)

GenerateSources("Elpida Standard Library Benchmarks" "Performs standard library benchmarks and outputs the results" STDLIB_BENCHMARKS_SOURCES)
add_executable(stdlib-benchmarks ${STDLIB_BENCHMARKS_SOURCES})
target_link_libraries(stdlib-benchmarks PUBLIC elpida-entry-point elpida-stdlib-benchmarks)

GenerateSources("Elpida Web Benchmarks" "Performs web benchmarks and outputs the results" WEB_BENCHMARKS_SOURCES)
add_executable(web-benchmarks ${WEB_BENCHMARKS_SOURCES})
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