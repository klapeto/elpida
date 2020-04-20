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

        ElpidaException.cpp

        Topology/ProcessorNode.cpp
        Topology/SystemTopology.cpp
        Topology/CpuFeature.cpp
        Topology/CpuInfo.cpp

        Utilities/CommandParser.cpp
        Utilities/FileSystem.cpp
        Utilities/Imaging/Image.cpp
        Utilities/Imaging/ImageEncoder.cpp
        Utilities/Logging/Logger.cpp
        Utilities/Logging/LogAppender.cpp
        Utilities/MemoryFile.cpp
        Utilities/NonCopyable.cpp
        Utilities/Imaging/Pixel.cpp
        Utilities/ValueUtilities.cpp
        Utilities/WindowsUtils.cpp
        Utilities/Plugin/Plugin.cpp
        Utilities/Plugin/TaskBatchesContainerPlugin.cpp
        Utilities/Uuid.cpp

        CommonTasks/AllocateMemory/AllocateMemory.cpp
        CommonTasks/Memory.cpp
        CommonTasks/MemoryChunk.cpp
        CommonTasks/NumaMemory.cpp
        CommonTasks/ReadFile/ReadFile.cpp
        CommonTasks/WriteFile/WriteFile.cpp
        CommonTasks/AllocateMemory/AllocateMemorySpecification.cpp

        Engine/Benchmark.cpp
        Engine/Configuration/TaskConfiguration.cpp
        Engine/Configuration/ConfigurationValue.cpp
        Engine/Configuration/ConfigurationValueBase.cpp
        Engine/Task/Task.cpp
        Engine/Task/TaskSpecification.cpp
        Engine/Task/TaskData.cpp
        Engine/Runner/BenchmarkRunner.cpp
        Engine/Result/BenchmarkResult.cpp
        Engine/Result/TaskResult.cpp
        Engine/Result/TaskMetrics.cpp
        Engine/BenchmarkScoreCalculator.cpp
        Engine/DefaultBenchmarkScoreCalculator.cpp
        Engine/Runner/EventArgs/BenchmarkEventArgs.cpp
        Engine/Runner/EventArgs/TaskEventArgs.cpp
        Engine/Task/MultiThreadTask.cpp CommonTasks/ReadFile/ReadFileSpecification.cpp ../../include/Elpida/CommonTasks/ReadFile/ReadFileSpecification.hpp CommonTasks/WriteFile/WriteFileSpecification.cpp ../../include/Elpida/CommonTasks/WriteFile/WriteFileSpecification.hpp Engine/Configuration/BenchmarkConfiguration.cpp ../../include/Elpida/Engine/Configuration/BenchmarkConfiguration.hpp Engine/Configuration/ConfigurationSpecification.cpp ../../include/Elpida/Engine/Configuration/ConfigurationSpecification.hpp ../../include/Elpida/Engine/Configuration/ConfigurationSpecificationGroups.cpp ../../include/Elpida/Engine/Configuration/ConfigurationSpecificationGroups.hpp ../../include/Elpida/Engine/Configuration/TaskConfigurationSpecifications.cpp ../../include/Elpida/Engine/Configuration/TaskConfigurationSpecifications.hpp Engine/Runner/BenchmarkRunRequest.cpp ../../include/Elpida/Engine/Runner/BenchmarkRunRequest.hpp)


set(ELPIDA_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

configure_file("${ELPIDA_INCLUDE_DIR}/Elpida/Config.hpp.in" "Elpida/Config.hpp")

add_library(elpida SHARED ${ELPIDA_SOURCES})

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