project("Elpida Benchmark Entry Point" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida Simple Benchmarking Framework"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_ENTRY_POINT_SOURCES
        Main.cpp
        ArgumentsHelper.cpp
        JsonFormatter.cpp
        DefaultFormatter.cpp
        ResultFormatter.cpp
)

add_library(elpida-entry-point ${ELPIDA_ENTRY_POINT_SOURCES})

target_link_libraries(elpida-entry-point PRIVATE elpida-core elpida-platform)