project(elpidaCore VERSION 0.0.1
        DESCRIPTION "Elpida Application Core"
        LANGUAGES CXX C)

set(ELPIDA_CORE_SOURCES
        Core/ElpidaMediator.cpp
        Core/DataUploader.cpp
        Core/Abstractions/ResultFormatter.cpp
        Core/JsonResultFormatter.cpp

        Models/Benchmarks/BenchmarksModel.cpp
        Models/Abstractions/CollectionModel.cpp
        Models/Abstractions/CollectionItem.cpp
        Models/Abstractions/EventArgs/CollectionChangedEventArgs.cpp
        Models/Abstractions/CollectionModelObserver.cpp
        Models/BenchmarkResultsModel.cpp
        Models/BenchmarkRunnerModel.cpp
        Models/BenchmarkConfigurationsCollectionModel.cpp
        Models/Abstractions/AssociativeModel/AssociativeModel.cpp
        Models/Abstractions/ListModel/ListModel.cpp
        Models/Abstractions/ListModel/ListModelItem.cpp
        Models/Abstractions/AssociativeModel/AssociativeItem.cpp
        Models/BenchmarkConfigurationModel.cpp
        Models/Screens/ScreensModel.cpp
        Models/Screens/ScreenItem.cpp
        Models/Benchmarks/BenchmarkGroup.cpp

        Controllers/BenchmarksController.cpp
        Controllers/BenchmarkRunnerController.cpp
        Controllers/BenchmarkConfigurationController.cpp
        Controllers/UploadController.cpp)

add_library(elpidaCore ${ELPIDA_CORE_SOURCES})

if (ELPIDA_DEBUG_BUILD EQUAL 1)
    add_definitions(-DTASK_BATCH_DEBUG_DIR="${TASK_BATCH_DEBUG_DIR}")
endif ()

target_link_libraries(elpidaCore elpida)
target_include_directories(elpidaCore PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(elpida PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

