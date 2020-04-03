set(ELPIDA_SOURCES
        CpuFeature.cpp
        CpuInfo.cpp
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
        Utilities/Image.cpp
        Utilities/ImageEncoder.cpp
        Utilities/Logger.cpp
        Utilities/MemoryFile.cpp
        Utilities/NonCopyable.cpp
        Utilities/Pixel.cpp
        Utilities/Singleton.cpp
        Utilities/ValueUtilities.cpp
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

install(TARGETS elpida DESTINATION Elpida)