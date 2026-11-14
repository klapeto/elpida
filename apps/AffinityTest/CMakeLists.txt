project("Elpida Affinity Test" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Compares single-thread benchmarks pinned to one processor with OS scheduling"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

add_executable(elpida-affinity-test Main.cpp)

target_link_libraries(elpida-affinity-test PUBLIC elpida-core elpida-platform elpida-application elpida-shared)

# Next to elpida-cli: it uses the same info dumper and Benchmarks directory
install(TARGETS elpida-affinity-test RUNTIME DESTINATION bin)
