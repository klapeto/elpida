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

# name, library, description, entry point
function(AddBenchmarkExecutable Executable Library Title Description EntryPoint)
    GenerateSources("${Title}" "${Description}" SOURCES)
    add_executable(${Executable} ${SOURCES})
    target_link_options(${Executable} PUBLIC -static-libgcc -static-libstdc++)
    target_link_libraries(${Executable} PUBLIC ${EntryPoint} ${Library})
    install(TARGETS ${Executable} RUNTIME DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})
endfunction()

if (ELPIDA_ENABLE_BENCHMARKS)
    AddBenchmarkExecutable(compilation-benchmarks elpida-compilation-benchmarks "Elpida Compilation Benchmarks"
            "Performs compilation benchmarks and outputs the results" elpida-entry-point)
    AddBenchmarkExecutable(compression-benchmarks elpida-compression-benchmarks "Elpida Compression Benchmarks"
            "Performs compression benchmarks and outputs the results" elpida-entry-point)
    AddBenchmarkExecutable(encryption-benchmarks elpida-encryption-benchmarks "Elpida Encryption Benchmarks"
            "Performs encryption benchmarks and outputs the results" elpida-entry-point)
    AddBenchmarkExecutable(image-benchmarks elpida-image-benchmarks "Elpida Image Benchmarks"
            "Performs image benchmarks and outputs the results" elpida-entry-point)
    AddBenchmarkExecutable(math-benchmarks elpida-math-benchmarks "Elpida Math Benchmarks"
            "Performs math benchmarks and outputs the results" elpida-entry-point)
    AddBenchmarkExecutable(memory-benchmarks elpida-memory-benchmarks "Elpida Memory Benchmarks"
            "Performs memory benchmarks and outputs the results" elpida-entry-point)
    AddBenchmarkExecutable(web-benchmarks elpida-web-benchmarks "Elpida Web Benchmarks"
            "Performs web benchmarks and outputs the results" elpida-entry-point)
endif ()

if (ELPIDA_ENABLE_STDLIB_BENCHMARKS)
    # Measures the platform's C library and allocator, so no musl-lite and no mimalloc
    # (docs/development/code-parity.md#the-stdlib-benchmarks)
    AddBenchmarkExecutable(stdlib-benchmarks elpida-stdlib-benchmarks "Elpida Standard Library Benchmarks"
            "Performs standard library benchmarks and outputs the results" elpida-entry-point-system-libc)
endif ()
