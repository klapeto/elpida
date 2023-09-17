#-------------------------------------------------
#
# Project created by QtCreator 2018-09-20T23:41:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = elpida-qt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	Dialogs/BenchmarkRunningDialog.cpp \
	Main.cpp \
    Views/BenchmarkConfigurationView/BenchmarkConfigurationView.cpp \
    Views/BenchmarkListView/BenchmarkListView.cpp \
    Views/BenchmarkRunnerControlsView/BenchmarkRunnerControlsView.cpp \
    Views/BenchmarkRunnerStatusView/BenchmarkRunnerStatusView.cpp \
    Views/CommonDialog/CommonDialog.cpp \
    Views/ConfigurationViews/FileInputView/FileInputView.cpp \
    Views/ConfigurationViews/TaskListItemView/TaskListItemView.cpp \
    MainWindow.cpp \
    Views/NavigationBarView/NavigationBarView.cpp \
    Views/QuickStartView/QuickStartView.cpp \
    Views/SystemInfoView/SystemInfoView.cpp \
    Views/BenchmarkResultsView/BenchmarkResultsView.cpp \
    Views/TopologyNodeFrame/TopologyNodeFrame.cpp \
    Views/TopologyView/TopologyView.cpp \
    Views/ConfigurationViews/ConfigurationView.cpp \
    Views/OsInfoView/OsInfoView.hpp \
    Views/ConfigurationViews/FileConfigurationView/FileConfigurationView.cpp \
    Views/ConfigurationViews/StringConfigurationView/StringConfigurationView.cpp \
    Views/ConfigurationViews/FloatConfigurationView/FloatConfigurationView.cpp \
    Views/ConfigurationViews/IntegerConfigurationView/IntegerConfigurationView.cpp \
    Views/BenchmarksView/BenchmarksView.cpp \
    Views/TopologyNodeView/TopologyNodeView.cpp \
    Views/CpuInfoView/CpuInfoView.cpp \
    Views/TimingInfoView/TimingInfoView.cpp \
    Views/MemoryInfoView/MemoryInfoView.cpp


HEADERS += \
    Dialogs/BenchmarkRunningDialog.hpp \
    Views/BenchmarkConfigurationView/BenchmarkConfigurationView.hpp \
    Views/BenchmarkListView/BenchmarkListView.hpp \
    Views/BenchmarkRunnerControlsView/BenchmarkRunnerControlsView.hpp \
    Views/BenchmarkRunnerStatusView/BenchmarkRunnerStatusView.hpp \
    Views/CommonDialog/CommonDialog.hpp \
    Views/ConfigurationViews/FileInputView/FileInputView.hpp \
    Views/ConfigurationViews/TaskListItemView/TaskListItemView.hpp \
    MainWindow.hpp \
    Views/NavigationBarView/NavigationBarView.hpp \
    Views/QuickStartView/QuickStartView.hpp \
    Views/SystemInfoView/SystemInfoView.hpp \
    Views/BenchmarkResultsView/BenchmarkResultsView.hpp \
    Views/TopologyNodeFrame/TopologyNodeFrame.hpp \
    Views/TopologyView/TopologyView.hpp \
    Views/ConfigurationViews/ConfigurationView.hpp \
    Views/OsInfoView/OsInfoView.hpp \
    Views/ConfigurationViews/FileConfigurationView/FileConfigurationView.hpp \
    Views/ConfigurationViews/StringConfigurationView/StringConfigurationView.hpp \
    Views/ConfigurationViews/FloatConfigurationView/FloatConfigurationView.hpp \
    Views/ConfigurationViews/IntegerConfigurationView/IntegerConfigurationView.hpp \
    Views/BenchmarksView/BenchmarksView.hpp \
    Views/TopologyNodeView/TopologyNodeView.hpp \
    Views/CpuInfoView/CpuInfoView.hpp \
    Views/TimingInfoView/TimingInfoView.hpp \
    Views/MemoryInfoView/MemoryInfoView.hpp


FORMS += \
    Dialogs/BenchmarkRunningDialog/BenchmarkRunningDialog.ui \
    Views/BenchmarkConfigurationView/BenchmarkConfigurationView.ui \
    Views/BenchmarkListView/BenchmarkListView.ui \
    Views/BenchmarkRunnerControlsView/BenchmarkRunnerControlsView.ui \
    Views/BenchmarkRunnerStatusView/BenchmarkRunnerStatusView.ui \
    Views/CommonDialog/CommonDialog.ui \
    Views/ConfigurationViews/FileInputView/FileInputView.ui \
    MainWindow.ui \
    Views/NavigationBarView/NavigationBarView.ui \
    Views/QuickStartView/QuickStartView.ui \
    Views/SystemInfoView/SystemInfoView.ui \
    Views/BenchmarkResultsView/BenchmarkResultsView.ui \
    Views/TopologyNodeFrame/TopologyNodeFrame.ui \
    Views/TopologyView/TopologyView.ui \
    Views/OsInfoView/OsInfoView.ui \
    Views/MemoryInfoView/MemoryInfoView.ui \
    Views/TimingInfoView/TimingInfoView.ui \
    Views/CpuInfoView/CpuInfoView.ui \
    Views/TopologyNodeView/TopologyNodeView.ui \
    Views/BenchmarksView/BenchmarksView.ui \
    Views/ConfigurationViews/IntegerConfigurationView/IntegerConfigurationView.ui \
    Views/ConfigurationViews/FloatConfigurationView/FloatConfigurationView.ui \
    Views/ConfigurationViews/StringConfigurationView/StringConfigurationView.ui \
    Views/ConfigurationViews/FileConfigurationView/FileConfigurationView.ui

RESOURCES += \
    Images/Images.qrc

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include

DISTFILES +=


