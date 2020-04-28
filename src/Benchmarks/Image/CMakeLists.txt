cmake_minimum_required(VERSION 3.10)

project(ImageTasks  VERSION 0.0.1
        DESCRIPTION "Image Tasks for Elpida"
        LANGUAGES CXX C)

set(IMAGE_TASKS_SOURCES
        ConvertToFloat.cpp
        ConvertToUInt8.cpp
        FloydSteinberg.cpp
        GrayscaleAverage.cpp
        ImageTaskBatch.cpp
        ImageTasksProperties.cpp
        PngDecoding.cpp
        PngEncoding.cpp
        Plugin.cpp
        Encoders/LibPngEncoder.cpp
        ImageTasksProperties.ui

        ${CMAKE_SOURCE_DIR}/include/TaskBatches/Image/ImageTasksProperties.hpp
        )


find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_library(imageTasks MODULE ${IMAGE_TASKS_SOURCES})

target_include_directories(imageTasks PRIVATE "${TASK_BATCHES_CONFIG_INCLUDE_DIR}")
target_link_libraries(imageTasks elpida Qt5::Widgets png)

install(TARGETS imageTasks DESTINATION ${ELPIDA_TASK_BATCH_INSTALL_DIR})