project("Elpida Web benchmarks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida web benchmarks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_WEB_BENCHMARKS_SOURCES
        Plugin.cpp
        DataParseBenchmark.cpp
        ParseJsonTask.cpp)

add_library(elpida-web-benchmarks SHARED ${ELPIDA_WEB_BENCHMARKS_SOURCES})

set_target_properties(elpida-web-benchmarks PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(elpida-web-benchmarks PRIVATE elpida-core elpida-common-tasks)

install(TARGETS elpida-web-benchmarks DESTINATION ${ELPIDA_BENCHMARK_INSTALL_DIR})
