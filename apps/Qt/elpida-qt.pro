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
	Main.cpp \
    Views/BenchmarkConfigurationView/BenchmarkConfigurationView.cpp \
    Views/BenchmarkListView/BenchmarkListView.cpp \
    Views/BenchmarkRunnerControlsView/BenchmarkRunnerControlsView.cpp \
    Views/BenchmarkRunnerStatusView/BenchmarkRunnerStatusView.cpp \
    Views/CommonDialog/CommonDialog.cpp \
    Views/ConfigurationViews/FileInputView/FileInputView.cpp \
    Views/ConfigurationViews/TaskListItemView/TaskListItemView.cpp \
    Views/LogsView/LogsView.cpp \
    Views/MainWindow/MainWindow.cpp \
    Views/NavigationBarView/NavigationBarView.cpp \
    Views/SystemInfoView/SystemInfoView.cpp \
    Views/BenchmarkResultsView/BenchmarkResultsView.cpp \
    Views/TopologyNodeFrame/TopologyNodeFrame.cpp \
    Views/TopologyView/TopologyView.cpp

HEADERS += \
    Views/BenchmarkConfigurationView/BenchmarkConfigurationView.hpp \
    Views/BenchmarkListView/BenchmarkListView.hpp \
    Views/BenchmarkRunnerControlsView/BenchmarkRunnerControlsView.hpp \
    Views/BenchmarkRunnerStatusView/BenchmarkRunnerStatusView.hpp \
    Views/CommonDialog/CommonDialog.hpp \
    Views/ConfigurationViews/FileInputView/FileInputView.hpp \
    Views/ConfigurationViews/TaskListItemView/TaskListItemView.hpp \
    Views/LogsView/LogsView.hpp \
    Views/MainWindow/MainWindow.hpp \
    Views/NavigationBarView/NavigationBarView.hpp \
    Views/SystemInfoView/SystemInfoView.hpp \
    Views/BenchmarkResultsView/BenchmarkResultsView.hpp \
    Views/TopologyNodeFrame/TopologyNodeFrame.hpp \
    Views/TopologyView/TopologyView.hpp

FORMS += \
    Views/BenchmarkConfigurationView/BenchmarkConfigurationView.ui \
    Views/BenchmarkListView/BenchmarkListView.ui \
    Views/BenchmarkRunnerControlsView/BenchmarkRunnerControlsView.ui \
    Views/BenchmarkRunnerStatusView/BenchmarkRunnerStatusView.ui \
    Views/CommonDialog/CommonDialog.ui \
    Views/ConfigurationViews/FileInputView/FileInputView.ui \
    Views/LogsView/LogsView.ui \
    Views/MainWindow/MainWindow.ui \
    Views/NavigationBarView/NavigationBarView.ui \
    Views/SystemInfoView/SystemInfoView.ui \
    Views/BenchmarkResultsView/BenchmarkResultsView.ui \
    Views/TopologyNodeFrame/TopologyNodeFrame.ui \
    Views/TopologyView/TopologyView.ui

RESOURCES += \
    Images/Images.qrc

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include

DISTFILES +=


