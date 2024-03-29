set(ELPIDA_SOURCES
        Event.cpp
        EventSubscription.cpp
        OffThreadExecutor.cpp
        SharedLibrary.cpp
        SharedLibraryLoader.cpp
        Timer.cpp
        EventsSubscriber.cpp
        ServiceProvider.cpp
        SharedStructuresProvider.cpp

        ElpidaException.cpp
        OperationCanceledException.cpp

        SystemInfo/ProcessorNode.cpp
        SystemInfo/SystemTopology.cpp
        SystemInfo/CpuFeature.cpp
        SystemInfo/CpuInfo.cpp
        SystemInfo/CpuCache.cpp
        SystemInfo/MemoryInfo.cpp
        SystemInfo/OsInfo.cpp
        SystemInfo/TimingInfo.cpp
        SystemInfo/TargetTimeCalculator.cpp

        SystemInfo/X86Info.cpp
        SystemInfo/ArmInfo.cpp

        Utilities/Logging/Logger.cpp
        Utilities/Logging/LogAppender.cpp
        Utilities/NonCopyable.cpp
        Utilities/ValueUtilities.cpp
        Utilities/Plugin/Plugin.cpp
        Utilities/Plugin/BenchmarksContainerPlugin.cpp
        Utilities/Uuid.cpp
        Utilities/Memory.cpp
        Utilities/NumaMemory.cpp
        Utilities/OsUtilities.cpp
        Utilities/Statistics.cpp

        CommonTasks/AllocateMemory/AllocateMemory.cpp
        CommonTasks/AllocateMemory/AllocateMemorySpecification.cpp
        CommonTasks/ReadFile/ReadFile.cpp
        CommonTasks/ReadFile/ReadFileSpecification.cpp
        CommonTasks/WriteFile/WriteFile.cpp
        CommonTasks/WriteFile/WriteFileSpecification.cpp

        Engine/Benchmark/Benchmark.cpp
        Engine/Configuration/Concrete/TaskConfiguration.cpp
        Engine/Configuration/Concrete/ConfigurationValue.cpp
        Engine/Configuration/Concrete/ConfigurationValueBase.cpp
        Engine/Task/Task.cpp
        Engine/Task/TaskSpecification.cpp
        Engine/Task/TaskAffinity.cpp
        Engine/Task/TaskThread.cpp
        Engine/Runner/BenchmarkRunner.cpp
        Engine/Result/BenchmarkResult.cpp
        Engine/Result/TaskResult.cpp
        Engine/Result/TaskMetrics.cpp
        Engine/Calculators/BenchmarkScoreCalculator.cpp
        Engine/Calculators/Benchmark/AccumulativeScoreCalculator.cpp
        Engine/Runner/EventArgs/BenchmarkEventArgs.cpp
        Engine/Runner/EventArgs/TaskEventArgs.cpp
        Engine/Task/MultiThreadTask.cpp
        Engine/Configuration/Concrete/BenchmarkConfiguration.cpp
        Engine/Configuration/Specification/ConfigurationSpecification.cpp
        Engine/Runner/BenchmarkRunRequest.cpp
        Engine/Configuration/Specification/ConfigurationSpecificationBase.cpp
        Engine/Task/TaskDataDto.cpp
        Engine/Data/RawTaskData.cpp
        Engine/Data/DataSpecification.cpp
        Engine/Result/ResultSpecification.cpp
        Engine/Task/TaskBuilder.cpp
        Engine/Data/DataAdapter.cpp
        Engine/Benchmark/BenchmarkTaskInstance.cpp
        Engine/Configuration/ConfigurationInstance.cpp
        Engine/Data/DataTransformer.cpp
        Engine/Calculators/Benchmark/AverageScoreCalculator.cpp
        Engine/Calculators/TaskResultCalculator.cpp
        Engine/Calculators/Task/ExclusiveHarmonicMeanTaskResultCalculator.cpp
        Engine/Runner/MicroTaskRunner.cpp
        Engine/Calculators/Task/MinimumTaskResultCalculator.cpp
        Engine/Runner/DefaultTaskRunner.cpp
        Engine/Result/ProcessedTaskResult.cpp
        Engine/Task/WorkloadTask.cpp
        Engine/Task/MicroTask.cpp
        Engine/Task/ExecutionParameters.cpp
        Engine/Result/BenchmarkScore.cpp
        Engine/Result/BenchmarkScoreSpecification.cpp
        Engine/Calculators/Benchmark/AverageThroughputScoreCalculator.cpp
)

set(ELPIDA_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

configure_file("${ELPIDA_INCLUDE_DIR}/Elpida/Config.hpp.in" "Elpida/Config.hpp")

if (MSYS OR MINGW)
    generate_product_version(
            ELPIDA_SOURCES
            NAME ${PROJECT_NAME}
            ICON ${ELPIDA_ROOT_DIRECTORY}/images/Elpida-icon.ico
            VERSION_MAJOR ${ELPIDA_VERSION_MAJOR}
            VERSION_MINOR ${ELPIDA_VERSION_MINOR}
            VERSION_PATCH ${ELPIDA_VERSION_REVISION}
            VERSION_REVISION ${ELPIDA_VERSION_BUILD}
            COMPANY_NAME ${ELPIDA_COMPANY_NAME}
            FILE_DESCRIPTION ${PROJECT_DESCRIPTION}
    )
endif()

add_library(elpida SHARED ${ELPIDA_SOURCES})

if (UNIX)
    include(FindThreads)
    include(${CMAKE_SOURCE_DIR}/cmake-modules/FindNUMA.cmake)
#    find_library(numa REQUIRED)
    find_package(Threads)
    target_link_libraries(elpida numa ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
endif()

include(FindHWLOC)

find_package(HWLOC REQUIRED)

target_link_libraries(elpida hwloc)

target_include_directories(elpida PUBLIC "${ELPIDA_INCLUDE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")

install(DIRECTORY ${ELPIDA_INCLUDE_DIR}/Elpida DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h*")
install(TARGETS elpida
        PUBLIC_HEADER DESTINATION ${ELPIDA_HEADER_INSTALL_DIR}
        LIBRARY DESTINATION ${ELPIDA_LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${ELPIDA_LIB_INSTALL_DIR}
        )