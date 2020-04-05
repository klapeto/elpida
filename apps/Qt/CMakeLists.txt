set(ELPIDA_QT_SOURCES
        ElpidaManager.cpp
        ListItemWithButton.cpp
        LogsDialog.cpp
        Main.cpp
        MainWindow.cpp
        MascotWidget.cpp
        TaskBatchesWidget.cpp
        TaskBatchProperties.cpp
        TopologyFrame.cpp
        TopologyWidget.cpp
        ListItemWithButton.ui
        MainWindow.ui
        TaskBatchProperties.ui
        TaskBatchesWidget.ui
        ListItemWithButton.ui
        LogsDialog.ui
        TopologyWidget.ui
        TopologyFrame.ui
        Images.qrc
        )


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

install(TARGETS elpida-qt RUNTIME)