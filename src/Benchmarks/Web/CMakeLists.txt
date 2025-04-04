project("Elpida Web benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida web benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_WEB_BENCHMARKS_SOURCES
        Plugin.cpp
        XmlParseBenchmark.cpp
        ParseJsonTask.cpp
        XmlGenerationTask.cpp
        JsonGenerationTask.cpp
        JsonParseBenchmark.cpp
        Base64.cpp
        Base64EncodeTask.cpp
        Base64EncodeBenchmark.cpp
        Base64DecodeTask.cpp
        Base64DecodeBenchmark.cpp
        RegexTask.cpp
        RegexBenchmark.cpp
)

add_executable(elpida-web-benchmarks ${ELPIDA_WEB_BENCHMARKS_SOURCES})

target_link_libraries(elpida-web-benchmarks PRIVATE elpida-core elpida-common-tasks elpida-entry-point boost_regex musl-lite)

install(TARGETS elpida-web-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})
