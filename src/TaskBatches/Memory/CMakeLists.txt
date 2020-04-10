project(MemoryTasks VERSION 0.0.1
        DESCRIPTION "Memory Tasks for Elpida"
        LANGUAGES CXX C)

set(MEMORY_TASKS_SOURCES
        Plugin.cpp
        WorkingSetSizes.cpp
        Latency/MemoryLatency.cpp
        Latency/MemoryLatencyTaskBatch.cpp
        Read/MemoryRead.cpp
        Read/MemoryReadTaskFactory.cpp
        Read/MultiThreadMemoryChunksRead.cpp
        Read/MultithreadMemoryChunksReadTaskBatch.cpp
        Ui/MemoryBandwidthChart.cpp
        Ui/MemoryLatencyChart.cpp
        Ui/MemoryReadProperties.cpp
        Ui/MemoryTasksProperties.cpp
        Ui/MemoryTasksPropertiesWithChart.cpp
        Ui/MemoryReadProperties.ui

        ${CMAKE_SOURCE_DIR}/include/TaskBatches/Memory/Ui/MemoryReadProperties.hpp
        )


find_package(Qt5 COMPONENTS Widgets Charts REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif ()

add_library(memoryTasks MODULE ${MEMORY_TASKS_SOURCES})

target_include_directories(memoryTasks PRIVATE "${TASK_BATCHES_CONFIG_INCLUDE_DIR}")
target_link_libraries(memoryTasks elpida Qt5::Widgets Qt5::Charts)
install(TARGETS memoryTasks
        CONFIGURATIONS Debug
        DESTINATION ${ELPIDA_INSTALL})
