set(ELPIDA_SOURCES
        Benchmark.cpp
        BenchmarkResult.cpp
        BenchmarkInfo.cpp
        TaskInfo.cpp
        DataInfo.cpp
        Task.cpp
        TaskData.cpp
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
        )

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

set_property(TARGET elpida PROPERTY POSITION_INDEPENDENT_CODE ON)