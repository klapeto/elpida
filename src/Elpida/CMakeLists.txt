set(ELPIDA_SOURCES
        Benchmark.cpp
        BenchmarkResult.cpp
        BenchmarkInfo.cpp
        TaskInfo.cpp
        Task.cpp
        RawTaskData.cpp
        NumaMemory.cpp
        NumaMemory.Unix.cpp
        TaskResult.cpp
        MicroTask.cpp
        ValueUtilities.cpp
        OverheadsInfo.cpp
        OsUtilities.cpp
        OsUtilities.Unix.cpp
        ThreadTask.cpp
        ThreadTask.Unix.cpp
        BenchmarkGroup.cpp
        TaskConfiguration.cpp
        ElpidaException.cpp Topology/TopologyInfo.cpp Topology/TopologyInfo.hpp Topology/TopologyNode.cpp Topology/TopologyNode.hpp Topology/CpuKind.cpp Topology/CpuKind.hpp EnvironmentInfo.cpp EnvironmentInfo.hpp Topology/CpuCacheNode.cpp Topology/CpuCacheNode.hpp Topology/ProcessingUnitNode.cpp Topology/ProcessingUnitNode.hpp Topology/NumaNode.cpp Topology/NumaNode.hpp Topology/MemoryPageType.cpp Topology/MemoryPageType.hpp AbstractTaskData.cpp AbstractTaskData.hpp DataUtilities.cpp DataUtilities.hpp)

configure_file("${PROJECT_SOURCE_DIR}/src/Elpida/Config.hpp.in" "Elpida/Config.hpp")

add_library(elpida ${ELPIDA_SOURCES})

target_include_directories(elpida PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(elpida PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(elpida PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

if (UNIX)
    include(FindThreads)
    include(${CMAKE_SOURCE_DIR}/cmake-modules/FindNUMA.cmake)
    #    find_library(numa REQUIRED)
    find_package(Threads)
    target_link_libraries(elpida numa ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
endif()

target_link_libraries(elpida hwloc)

set_property(TARGET elpida PROPERTY POSITION_INDEPENDENT_CODE ON)