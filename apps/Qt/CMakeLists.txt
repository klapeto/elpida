set(ELPIDA_QT_SOURCES
        Views/LogsWidget/LogsWidget.cpp
        Views/MainWindow/MainWindow.cpp
        Views/MascotWidget/MascotWidget.cpp
        Views/TopologyNodeFrame/TopologyNodeFrame.cpp
        Views/TopologyWidget/TopologyWidget.cpp
        Views/SystemInfoWidget/SystemInfoWidget.cpp
        Views/BenchmarkListView/BenchmarkListView.cpp
        Views/TaskResultsWidget/TaskResultsWidget.cpp
        Views/CommonDialog/CommonDialog.cpp
        Views/TaskBatchRunnerStatusView/TaskBatchRunnerStatusView.cpp
        Views/TaskBatchRunnerControlsView/TaskBatchRunnerControlsView.cpp

        Views/MainWindow/MainWindow.ui
        Views/BenchmarkListView/BenchmarkListView.ui
        Views/LogsWidget/LogsWidget.ui
        Views/TopologyWidget/TopologyWidget.ui
        Views/TopologyNodeFrame/TopologyNodeFrame.ui
        Views/SystemInfoWidget/SystemInfoWidget.ui
        Views/TaskResultsWidget/TaskResultsWidget.ui
        Views/CommonDialog/CommonDialog.ui
        Views/TaskBatchRunnerStatusView/TaskBatchRunnerStatusView.ui
        Views/TaskBatchRunnerControlsView/TaskBatchRunnerControlsView.ui

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

        Controllers/BenchmarksController.cpp
        Controllers/BenchmarkRunnerController.cpp
        Controllers/MainViewController.cpp
        QCustomApplication.cpp
        Models/BenchmarkConfigurationsModel.cpp Models/BenchmarkConfigurationsModel.hpp Models/Abstractions/AssociativeModel/AssociativeModel.cpp Models/Abstractions/AssociativeModel/AssociativeModel.hpp Models/Abstractions/ListModel/ListModel.cpp Models/Abstractions/ListModel/ListModel.hpp Models/Abstractions/ListModel/ListModelItem.cpp Models/Abstractions/ListModel/ListModelItem.hpp)


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