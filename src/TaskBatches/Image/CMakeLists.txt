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
        ImageTasksProperties.ui)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_library(imageTasks MODULE ${IMAGE_TASKS_SOURCES})

target_include_directories(imageTasks PRIVATE "${TASK_BATCHES_CONFIG_INCLUDE_DIR}")
target_link_libraries(imageTasks elpida generalTasks Qt5::Widgets png)

install(TARGETS imageTasks DESTINATION Elpida/Tasks)