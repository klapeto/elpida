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
        Topology/ProcessingUnitNode.Unix.cpp
        Topology/ProcessingUnitNote.Windows.cpp
        ThreadPool/Functor.cpp
        ThreadPool/ThreadPool.cpp
        ThreadPool/ThreadPoolThread.cpp
        BlockingCollection.cpp
        BenchmarkRunContext.cpp
)

configure_file("${PROJECT_SOURCE_DIR}/Config.hpp.in" "Elpida/Core/Config.hpp")

add_library(elpida-core ${ELPIDA_CORE_SOURCES})

add_library(overheads-calculator TimingCalculator.cpp DummyClass.cpp)
#target_link_libraries(overheads-calculator elpida-core)
target_compile_options(overheads-calculator PRIVATE -O3 -fno-dce -fno-tree-dce)

target_include_directories(elpida-core PUBLIC ${ELPIDA_ROOT_DIRECTORY}/include)
target_include_directories(elpida-core PUBLIC ${ELPIDA_ROOT_DIRECTORY}/src)
target_include_directories(elpida-core PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

if (UNIX)
    include(FindThreads)
    include(FindNUMA)
    find_package(Threads)
    target_link_libraries(elpida-core numa ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
endif()

target_link_libraries(elpida-core overheads-calculator)

set_property(TARGET elpida-core PROPERTY POSITION_INDEPENDENT_CODE ON)