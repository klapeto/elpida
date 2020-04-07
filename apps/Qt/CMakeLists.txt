set(ELPIDA_QT_SOURCES
        Core/ElpidaManager.cpp
        Ui/ListItemWithButton/ListItemWithButton.cpp
        Ui/LogsDialog/LogsDialog.cpp
        Main.cpp
        Ui/MainWindow/MainWindow.cpp
        Ui/MascotWidget/MascotWidget.cpp
        Ui/TaskBatchesWidget/TaskBatchesWidget.cpp
        Ui/TaskBatchesProperties/TaskBatchProperties.cpp
        Ui/TopologyFrame/TopologyFrame.cpp
        Ui/TopologyWidget/TopologyWidget.cpp
        Ui/ListItemWithButton/ListItemWithButton.ui
        Ui/MainWindow/MainWindow.ui
        Ui/TaskBatchesProperties/TaskBatchProperties.ui
        Ui/TaskBatchesWidget/TaskBatchesWidget.ui
        Ui/ListItemWithButton/ListItemWithButton.ui
        Ui/LogsDialog/LogsDialog.ui
        Ui/TopologyWidget/TopologyWidget.ui
        Ui/TopologyFrame/TopologyFrame.ui
        Images/Images.qrc
        Core/Abstractions/Component.hpp Core/Abstractions/ApplicationEvent.hpp Core/Abstractions/ComponentAdapter.hpp Core/Abstractions/VisitableComponent.hpp Core/Mediator.cpp Core/Mediator.hpp Core/LogComponent.cpp Core/LogComponent.hpp)


find_package(Qt5 COMPONENTS Widgets Charts REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_executable(elpida-qt ${ELPIDA_QT_SOURCES})

target_include_directories(elpida-qt PRIVATE "${TASK_BATCHES_CONFIG_INCLUDE_DIR}")
target_link_libraries(elpida-qt elpida Qt5::Widgets Qt5::Charts)
