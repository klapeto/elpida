set(ELPIDA_SOURCES
        Event.cpp
        EventSubscription.cpp
        MemoryInfo.cpp
        OffThreadExecutor.cpp
        SharedLibrary.cpp
        SharedLibraryLoader.cpp
        Engine/Task/TaskAffinity.cpp
        Engine/Task/TaskThread.cpp
        Timer.cpp
        EventsSubscriber.cpp

        ElpidaException.cpp

        Topology/ProcessorNode.cpp
        Topology/SystemTopology.cpp
        Topology/CpuFeature.cpp
        Topology/CpuInfo.cpp

        Utilities/FileSystem.cpp
        Utilities/Imaging/Image.cpp
        Utilities/Logging/Logger.cpp
        Utilities/Logging/LogAppender.cpp
        Utilities/MemoryFile.cpp
        Utilities/NonCopyable.cpp
        Utilities/Imaging/Pixel.cpp
        Utilities/ValueUtilities.cpp
        Utilities/Plugin/Plugin.cpp
        Utilities/Plugin/BenchmarksContainerPlugin.cpp
        Utilities/Uuid.cpp
        Utilities/Memory.cpp
        Utilities/NumaMemory.cpp
        Utilities/OsUtilities.cpp

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
        Utilities/RawData.cpp
        Engine/Runner/BenchmarkRunner.cpp
        Engine/Result/BenchmarkResult.cpp
        Engine/Result/TaskResult.cpp
        Engine/Result/TaskMetrics.cpp
        Engine/BenchmarkScoreCalculator.cpp
        Engine/DefaultBenchmarkScoreCalculator.cpp
        Engine/Runner/EventArgs/BenchmarkEventArgs.cpp
        Engine/Runner/EventArgs/TaskEventArgs.cpp
        Engine/Task/MultiThreadTask.cpp
        Engine/Configuration/Concrete/BenchmarkConfiguration.cpp
        Engine/Configuration/Specification/ConfigurationSpecification.cpp
        Engine/Runner/BenchmarkRunRequest.cpp
        Engine/Configuration/Specification/ConfigurationSpecificationBase.cpp
        Engine/Task/TaskDataDto.cpp
        Engine/Data/PassiveTaskData.cpp
        Engine/Data/ActiveTaskData.cpp
        Engine/Data/DataSpecification.cpp
        Engine/Result/ResultSpecification.cpp
        Engine/Task/TaskBuilder.cpp
        Engine/Data/DataAdapter.cpp
        Engine/Benchmark/BenchmarkTaskInstance.cpp
        Engine/Configuration/ConfigurationInstance.cpp
        Engine/Data/DataPropertiesTransformer.cpp
        )


set(ELPIDA_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

configure_file("${ELPIDA_INCLUDE_DIR}/Elpida/Config.hpp.in" "Elpida/Config.hpp")

add_library(elpida SHARED ${ELPIDA_SOURCES} Utilities/OsUtilities.cpp ../../include/Elpida/Utilities/OsUtilities.hpp)

if (UNIX)
    include(FindThreads)
    find_library(numa REQUIRED)
    find_library(dl REQUIRED)
    find_package(Threads)
    target_link_libraries(elpida numa dl ${CMAKE_THREAD_LIBS_INIT})
endif()

target_link_libraries(elpida hwloc)

target_include_directories(elpida PUBLIC "${ELPIDA_INCLUDE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")

install(DIRECTORY ${ELPIDA_INCLUDE_DIR}/Elpida DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h*")
install(TARGETS elpida
        PUBLIC_HEADER DESTINATION ${ELPIDA_HEADER_INSTALL_DIR}
        LIBRARY DESTINATION ${ELPIDA_LIB_INSTALL_DIR})