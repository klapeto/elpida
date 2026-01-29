project("Elpida Benchmark Entry Point" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida Simple Benchmarking Framework"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)


add_library(elpida-entry-point
        Main.cpp
        ArgumentsHelper.cpp
        JsonFormatter.cpp
        DefaultFormatter.cpp
        ResultFormatter.cpp
)

target_link_libraries(elpida-entry-point PUBLIC elpida-core elpida-platform)