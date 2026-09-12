option(ELPIDA_ENABLE_QT "Whether to enable the QT application" OFF)

if (ELPIDA_ENABLE_QT)
    add_subdirectory(Qt)
endif ()

option(ELPIDA_ENABLE_CLI "Whether to enable the CLI application" ON)

if (ELPIDA_ENABLE_CLI)
    add_subdirectory(CLI)
endif ()

if (ELPIDA_ENABLE_QT OR ELPIDA_ENABLE_CLI)
    add_subdirectory(Shared)
    add_subdirectory(Application)
    add_subdirectory(InfoDumper)
    add_subdirectory(BackEndInfoDumper)
    add_subdirectory(Benchmarks)
endif ()