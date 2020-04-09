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
        Ui/SystemInfoWidget/SystemInfoWidget.cpp
        Ui/ListItemWithButton/ListItemWithButton.ui
        Ui/MainWindow/MainWindow.ui
        Ui/TaskBatchesProperties/TaskBatchProperties.ui
        Ui/TaskBatchesWidget/TaskBatchesWidget.ui
        Ui/ListItemWithButton/ListItemWithButton.ui
        Ui/LogsDialog/LogsDialog.ui
        Ui/TopologyWidget/TopologyWidget.ui
        Ui/TopologyFrame/TopologyFrame.ui
        Ui/SystemInfoWidget/SystemInfoWidget.ui
        Images/Images.qrc
        Core/Abstractions/Command.hpp
        Core/Abstractions/CommandHandler.hpp
        Core/Abstractions/Mediator.hpp
        Core/ElpidaMediator.cpp
        Core/ElpidaMediator.hpp
        Core/Abstractions/TypedCommand.hpp
        Core/Commands/ShowLogsDialogCommand.hpp
        Core/Commands/ShowAboutDialogCommand.hpp
        Core/Abstractions/CommandForwardDeclarations.hpp
        Core/Commands/ExitApplicationCommand.hpp
        Core/QModelLogAppender.cpp
        Core/QModelLogAppender.hpp)


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
