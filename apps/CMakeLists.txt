option(ELPIDA_ENABLE_QT "Whether to enable the QT application" OFF)

if (ELPIDA_ENABLE_QT)
    add_subdirectory(Qt)
endif ()

option(ELPIDA_ENABLE_CLI "Whether to enable the CLI application" OFF)

if (ELPIDA_ENABLE_CLI)
    add_subdirectory(CLI)
endif ()

option(ELPIDA_ENABLE_APPLICATION_LIBRARIES "Whether to enable the application libraries" OFF)

if (ELPIDA_ENABLE_QT OR ELPIDA_ENABLE_CLI OR ELPIDA_ENABLE_APPLICATION_LIBRARIES)
    add_subdirectory(Shared)
    add_subdirectory(Application)
endif ()

if (ELPIDA_ENABLE_QT OR ELPIDA_ENABLE_CLI)
    add_subdirectory(InfoDumper)
    add_subdirectory(BackEndInfoDumper)
    add_subdirectory(Benchmarks)
endif ()
