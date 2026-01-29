project("Elpida Web benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida web benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)


add_library(elpida-web-benchmarks
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

target_link_libraries(elpida-web-benchmarks PUBLIC elpida-core elpida-common-tasks boost_regex musl-lite)

if (ELPIDA_INSTALL_PREDEFINED_BENCHMARK_LIBRARIES)
    install(TARGETS elpida-web-benchmarks ARCHIVE)
endif ()