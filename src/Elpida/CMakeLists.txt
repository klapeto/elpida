set(ELPIDA_SOURCES
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

        CommonTasks/AlignedMemory.cpp
        CommonTasks/AllocateMemory.cpp
        CommonTasks/Memory.cpp
        CommonTasks/MemoryChunk.cpp
        CommonTasks/NumaAllocatePerThread.cpp
        CommonTasks/NumaMemory.cpp
        CommonTasks/ReadFile.cpp
        CommonTasks/UnalignedMemory.cpp
        CommonTasks/WriteFile.cpp
        Utilities/WindowsUtils.cpp)


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