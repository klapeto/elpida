project("Elpida Benchmark Executor" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Executes benchmarks for Elpida"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_EXECUTOR_SOURCES
        Main.cpp
        )


add_executable(elpida-executor ${ELPIDA_EXECUTOR_SOURCES})

target_include_directories(elpida-executor PRIVATE "${TASK_BATCHES_CONFIG_INCLUDE_DIR}")
target_link_libraries(elpida-executor elpida elpidaCore)

install(TARGETS elpida-executor RUNTIME DESTINATION ${ELPIDA_APP_INSTALL_DIR})