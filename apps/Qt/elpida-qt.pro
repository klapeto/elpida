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
    Views/MainWindow/MainWindow.cpp \
    Views/NavigationBarView/NavigationBarView.cpp \
    Views/QuickStartView/QuickStartView.cpp \
    Views/SystemInfoView/SystemInfoView.cpp \
    Views/BenchmarkResultsView/BenchmarkResultsView.cpp \
    Views/TopologyNodeFrame/TopologyNodeFrame.cpp \
    Views/TopologyView/TopologyView.cpp

HEADERS += \
    Dialogs/BenchmarkRunningDialog.hpp \
    Views/BenchmarkConfigurationView/BenchmarkConfigurationView.hpp \
    Views/BenchmarkListView/BenchmarkListView.hpp \
    Views/BenchmarkRunnerControlsView/BenchmarkRunnerControlsView.hpp \
    Views/BenchmarkRunnerStatusView/BenchmarkRunnerStatusView.hpp \
    Views/CommonDialog/CommonDialog.hpp \
    Views/ConfigurationViews/FileInputView/FileInputView.hpp \
    Views/ConfigurationViews/TaskListItemView/TaskListItemView.hpp \
    Views/MainWindow/MainWindow.hpp \
    Views/NavigationBarView/NavigationBarView.hpp \
    Views/QuickStartView/QuickStartView.hpp \
    Views/SystemInfoView/SystemInfoView.hpp \
    Views/BenchmarkResultsView/BenchmarkResultsView.hpp \
    Views/TopologyNodeFrame/TopologyNodeFrame.hpp \
    Views/TopologyView/TopologyView.hpp

FORMS += \
    Dialogs/BenchmarkRunningDialog._ui \
    Views/BenchmarkConfigurationView/BenchmarkConfigurationView._ui \
    Views/BenchmarkListView/BenchmarkListView._ui \
    Views/BenchmarkRunnerControlsView/BenchmarkRunnerControlsView._ui \
    Views/BenchmarkRunnerStatusView/BenchmarkRunnerStatusView._ui \
    Views/CommonDialog/CommonDialog._ui \
    Views/ConfigurationViews/FileInputView/FileInputView._ui \
    Views/MainWindow/MainWindow._ui \
    Views/NavigationBarView/NavigationBarView._ui \
    Views/QuickStartView/QuickStartView._ui \
    Views/SystemInfoView/SystemInfoView._ui \
    Views/BenchmarkResultsView/BenchmarkResultsView._ui \
    Views/TopologyNodeFrame/TopologyNodeFrame._ui \
    Views/TopologyView/TopologyView._ui

RESOURCES += \
    Images/Images.qrc

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include

DISTFILES +=


