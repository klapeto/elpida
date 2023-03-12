project("Elpida Benchmark Executor" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Executes benchmarks for Elpida"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_EXECUTOR_SOURCES
        Main.cpp
        ArgumentsHelper.cpp
        ResultFormatter.cpp
        DefaultFormatter.cpp
        )


add_executable(elpida-executor ${ELPIDA_EXECUTOR_SOURCES})
target_link_libraries(elpida-executor elpida-core elpida-platform)

install(TARGETS elpida-executor RUNTIME)