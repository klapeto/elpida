project("Elpida Benchmark Metadata Dumper" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Dumps the metadata from all benchmarks to a JSON file"
        LANGUAGES CXX C)


set(ELPIDA_DUMPER_SOURCES
        Main.cpp
        )


add_definitions(-DTASK_BATCH_DEBUG_DIR="${TASK_BATCH_DEBUG_DIR}")

add_executable(elpida-dumper ${ELPIDA_DUMPER_SOURCES})

target_include_directories(elpida-dumper PRIVATE "${TASK_BATCHES_CONFIG_INCLUDE_DIR}")

target_link_libraries(elpida-dumper elpida elpidaCore)