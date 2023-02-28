set(ELPIDA_SOURCES
        Benchmark.cpp
        BenchmarkResult.cpp
        BenchmarkInfo.cpp
        TaskInfo.cpp
        DataInfo.cpp
        Task.cpp
        Test/MemoryLatencyBenchmark.cpp
        TaskData.cpp
        NumaAllocator.cpp
        NumaAllocator.Unix.cpp
        Test/MemoryLatencyTask.cpp
        TaskResult.cpp TaskResult.hpp MicroTask.cpp MicroTask.hpp ValueUtilities.cpp ValueUtilities.hpp OverheadsInfo.cpp OverheadsInfo.hpp OsUtilities.cpp OsUtilities.hpp OsUtilities.Unix.cpp ThreadTask.Unix.cpp Test/MemoryReadBandwidthTask.cpp Test/MemoryReadBandwidthTask.hpp Test/MemoryReadBandwidthBenchmark.cpp Test/MemoryReadBandwidthBenchmark.hpp ThreadTask.cpp ThreadTask.hpp)

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


target_compile_options(elpida PRIVATE -O3)