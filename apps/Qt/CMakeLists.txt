set(ELPIDA_QT_SOURCES
        Views/LogsView/LogsView.cpp
        Views/MainWindow/MainWindow.cpp
        Views/TopologyNodeFrame/TopologyNodeFrame.cpp
        Views/TopologyView/TopologyView.cpp
        Views/SystemInfoView/SystemInfoView.cpp
        Views/BenchmarkListView/BenchmarkListView.cpp
        Views/BenchmarkResultsView/BenchmarkResultsView.cpp
        Views/CommonDialog/CommonDialog.cpp
        Views/BenchmarkRunnerStatusView/BenchmarkRunnerStatusView.cpp
        Views/BenchmarkRunnerControlsView/BenchmarkRunnerControlsView.cpp
        Views/BenchmarkConfigurationView/BenchmarkConfigurationView.cpp
        Views/ConfigurationViews/FileInputView/FileInputView.cpp

        Views/MainWindow/MainWindow.ui
        Views/BenchmarkListView/BenchmarkListView.ui
        Views/LogsView/LogsView.ui
        Views/TopologyView/TopologyView.ui
        Views/TopologyNodeFrame/TopologyNodeFrame.ui
        Views/SystemInfoView/SystemInfoView.ui
        Views/BenchmarkResultsView/BenchmarkResultsView.ui
        Views/CommonDialog/CommonDialog.ui
        Views/BenchmarkRunnerStatusView/BenchmarkRunnerStatusView.ui
        Views/BenchmarkRunnerControlsView/BenchmarkRunnerControlsView.ui
        Views/BenchmarkConfigurationView/BenchmarkConfigurationView.ui
        Views/ConfigurationViews/FileInputView/FileInputView.ui

        Main.cpp

        Images/Images.qrc

        Core/ElpidaMediator.cpp
        Core/QModelLogAppender.cpp

        Models/BenchmarksModel.cpp
        Models/Abstractions/CollectionModel.cpp
        Models/Abstractions/CollectionItem.cpp
        Models/Abstractions/EventArgs/CollectionChangedEventArgs.cpp
        Models/Abstractions/CollectionModelObserver.cpp
        Models/BenchmarkResultsModel.cpp
        Models/BenchmarkRunnerModel.cpp
        Models/BenchmarkConfigurationsModel.cpp
        Models/Abstractions/AssociativeModel/AssociativeModel.cpp
        Models/Abstractions/ListModel/ListModel.cpp
        Models/Abstractions/ListModel/ListModelItem.cpp

        Controllers/BenchmarksController.cpp
        Controllers/BenchmarkRunnerController.cpp
        Controllers/MainViewController.cpp
        QCustomApplication.cpp
        )


find_package(Qt5 COMPONENTS Widgets Charts REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif ()

if (ELPIDA_DEBUG_BUILD EQUAL 1)
    message("${TASK_BATCH_DEBUG_DIR}")
    add_definitions(-DTASK_BATCH_DEBUG_DIR="${TASK_BATCH_DEBUG_DIR}")
endif ()


add_executable(elpida-qt ${ELPIDA_QT_SOURCES})

target_include_directories(elpida-qt PRIVATE "${TASK_BATCHES_CONFIG_INCLUDE_DIR}")
target_link_libraries(elpida-qt elpida Qt5::Widgets Qt5::Charts)

install(TARGETS elpida-qt RUNTIME DESTINATION ${ELPIDA_APP_INSTALL_DIR})