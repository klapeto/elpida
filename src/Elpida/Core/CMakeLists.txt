project("Elpida Core Library" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida Simple Benchmarking Framework"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_CORE_SOURCES
        Benchmark.cpp
        BenchmarkResult.cpp
        BenchmarkInfo.cpp
        TaskInfo.cpp
        Task.cpp
        RawTaskData.cpp
        TaskResult.cpp
        MicroTask.cpp
        ValueUtilities.cpp
        TimingInfo.cpp
        ThreadTask.cpp
        BenchmarkGroup.cpp
        TaskConfiguration.cpp
        ElpidaException.cpp
        EnvironmentInfo.cpp
        AbstractTaskData.cpp
        DataUtilities.cpp
        Allocator.cpp
        DefaultAllocator.cpp
        OsInfo.cpp
        MemoryInfo.cpp
        CpuInfo.cpp
        Pool.cpp

        Topology/TopologyInfo.cpp
        Topology/TopologyNode.cpp
        Topology/CpuKind.cpp
        Topology/CpuCacheNode.cpp
        Topology/ProcessingUnitNode.cpp
        Topology/NumaNode.cpp
        Topology/MemoryPageType.cpp
        AllocatorFactory.cpp
        DefaultAllocatorFactory.cpp
        BenchmarkRunContext.cpp
        Memory.cpp
        MemoryStream.cpp
        IterationMicroTask.cpp
)

configure_file("${PROJECT_SOURCE_DIR}/Config.hpp.in" "Elpida/Core/Config.hpp")

add_library(elpida-core ${ELPIDA_CORE_SOURCES})

add_library(overheads-calculator TimingCalculator.cpp)
target_compile_options(overheads-calculator PRIVATE -O3 -fno-dce -fno-tree-dce -fno-unroll-loops)

if (UNIX)
    include(FindThreads)
    find_package(Threads)
    target_link_libraries(elpida-core PUBLIC ${CMAKE_THREAD_LIBS_INIT})
endif()

target_include_directories(elpida-core PUBLIC ${ELPIDA_ROOT_DIRECTORY}/include)
target_include_directories(elpida-core PUBLIC ${ELPIDA_ROOT_DIRECTORY}/src)
target_include_directories(elpida-core PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(elpida-core PUBLIC overheads-calculator)