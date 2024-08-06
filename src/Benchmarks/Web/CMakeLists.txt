set(ELPIDA_WEB_BENCHMARKS_SOURCES
        Plugin.cpp
        XmlParseBenchmark.cpp
        ParseJsonTask.cpp
        XmlGenerationTask.cpp
        JsonGenerationTask.cpp
        JsonParseBenchmark.cpp
        JsonParseBenchmark.hpp)

add_executable(elpida-web-benchmarks ${ELPIDA_WEB_BENCHMARKS_SOURCES})

target_link_libraries(elpida-web-benchmarks PRIVATE musl elpida-core elpida-common-tasks elpida-entry-point)

install(TARGETS elpida-web-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})
