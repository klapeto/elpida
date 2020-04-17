set(ELPIDA_QT_SOURCES
        Ui/LogsWidget/LogsWidget.cpp
        Ui/MainWindow/MainWindow.cpp
        Ui/MascotWidget/MascotWidget.cpp
        Ui/TopologyNodeFrame/TopologyNodeFrame.cpp
        Ui/TopologyWidget/TopologyWidget.cpp
        Ui/SystemInfoWidget/SystemInfoWidget.cpp
        Ui/TaskBatchesListWidget/TaskBatchesListWidget.cpp
        Ui/TaskResultsWidget/TaskResultsWidget.cpp
        Ui/CommonDialog/CommonDialog.cpp

        Ui/MainWindow/MainWindow.ui
        Ui/TaskBatchesListWidget/TaskBatchesListWidget.ui
        Ui/LogsWidget/LogsWidget.ui
        Ui/TopologyWidget/TopologyWidget.ui
        Ui/TopologyNodeFrame/TopologyNodeFrame.ui
        Ui/SystemInfoWidget/SystemInfoWidget.ui
        Ui/TaskResultsWidget/TaskResultsWidget.ui
        Ui/CommonDialog/CommonDialog.ui

        Main.cpp

        Images/Images.qrc

        Core/ElpidaMediator.cpp
        Core/QModelLogAppender.cpp

        Models/TaskBatchesModel.cpp
        Models/Abstractions/CollectionModel.cpp
        Models/Abstractions/CollectionItem.cpp
        Models/Abstractions/EventArgs/CollectionChangedEventArgs.cpp
        Models/Abstractions/CollectionModelObserver.cpp
        Models/TaskRunResultsModel.cpp

        Controllers/TaskBatchesController.cpp
 )


find_package(Qt5 COMPONENTS Widgets Charts REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

if (ELPIDA_DEBUG_BUILD EQUAL 1)
    message("${TASK_BATCH_DEBUG_DIR}")
    add_definitions(-DTASK_BATCH_DEBUG_DIR="${TASK_BATCH_DEBUG_DIR}")
endif()


add_executable(elpida-qt ${ELPIDA_QT_SOURCES})

target_include_directories(elpida-qt PRIVATE "${TASK_BATCHES_CONFIG_INCLUDE_DIR}")
target_link_libraries(elpida-qt elpida Qt5::Widgets Qt5::Charts)

install(TARGETS elpida-qt RUNTIME DESTINATION ${ELPIDA_APP_INSTALL_DIR})