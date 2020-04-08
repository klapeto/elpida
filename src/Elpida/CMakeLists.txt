set(ELPIDA_SOURCES
        Topology/CpuFeature.cpp
        Topology/CpuInfo.cpp
        Event.cpp
        EventSubscription.cpp
        MemoryInfo.cpp
        MultiThreadTask.cpp
        OffThreadExecutor.cpp
        Runner.cpp
        SharedLibrary.cpp
        SharedLibraryLoader.cpp
        Task.cpp
        TaskBatch.cpp
        TaskAffinity.cpp
        TaskBatchWrapper.cpp
        TaskFactory.cpp
        TaskMetrics.cpp
        TaskRunResult.cpp
        TaskThread.cpp
        TaskThroughput.cpp
        Timer.cpp

        Exceptions/ElpidaException.cpp
        Exceptions/IOException.cpp
        Exceptions/OutOfRangeException.cpp
        Exceptions/ParserException.cpp

        Topology/ProcessorNode.cpp
        Topology/SystemTopology.cpp

        Utilities/CommandParser.cpp
        Utilities/FileSystem.cpp
        Utilities/Imaging/Image.cpp
        Utilities/Imaging/ImageEncoder.cpp
        Utilities/Logger.cpp
        Utilities/MemoryFile.cpp
        Utilities/NonCopyable.cpp
        Utilities/Imaging/Pixel.cpp
        Utilities/ValueUtilities.cpp

        CommonTasks/AlignedMemory.cpp
        CommonTasks/AllocateMemory.cpp
        CommonTasks/Memory.cpp
        CommonTasks/MemoryChunk.cpp
        CommonTasks/NumaAllocatePerThread.cpp
        CommonTasks/NumaMemory.cpp
        CommonTasks/ReadFile.cpp
        CommonTasks/UnalignedMemory.cpp
        CommonTasks/WriteFile.cpp
        )


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