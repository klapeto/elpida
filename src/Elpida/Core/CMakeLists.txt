project("Elpida Core Library" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida Simple Benchmarking Framework"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

add_library(elpida-core
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

add_library(elpida-overheads-calculator TimingCalculator.cpp)

target_compile_options(elpida-overheads-calculator PRIVATE -O3)

if (UNIX)
    include(FindThreads)
    find_package(Threads)
    target_link_libraries(elpida-core PUBLIC ${CMAKE_THREAD_LIBS_INIT})
endif()

target_include_directories(elpida-core PUBLIC
        ${ELPIDA_ROOT_DIRECTORY}/include
        ${ELPIDA_ROOT_DIRECTORY}/src
        ${CMAKE_CURRENT_BINARY_DIR}
)
if (NOT ANDROID)
    include(${ELPIDA_ROOT_DIRECTORY}/cmake-modules/FindHWLOC.cmake)
    target_include_directories(elpida-core PRIVATE ${HWLOC_INCLUDE_DIR})
endif ()

target_link_libraries(elpida-core PUBLIC hwloc)

if (ELPIDA_INSTALL_CORE_LIBRARIES)
    install(TARGETS elpida-core elpida-overheads-calculator ARCHIVE)
endif ()