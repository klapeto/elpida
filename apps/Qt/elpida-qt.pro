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
	Dialogs/BenchmarkRunningDialog/BenchmarkRunningDialog.cpp \
	Views/BenchmarkRunConfigurationView/BenchmarkRunConfigurationView.cpp \
	Views/ConfigurationViews/ConfigurationView.cpp \
    Views/ConfigurationViews/FileConfigurationView/FileConfigurationView.cpp \
    Views/ConfigurationViews/FloatConfigurationView/FloatConfigurationView.cpp \
    Views/ConfigurationViews/IntegerConfigurationView/IntegerConfigurationView.cpp \
    Views/ConfigurationViews/StringConfigurationView/StringConfigurationView.cpp \
    Views/CpuInfoView/CpuInfoView.cpp \
    Views/CustomBenchmarkConfigurationView/CustomBenchmarkConfigurationView.cpp \
    Views/CustomBenchmarkResultsView/CustomBenchmarkResultsView.cpp \
    Views/CustomBenchmarkView/CustomBenchmarkView.cpp \
    Views/FullBenchmarkView/FullBenchmarkView.cpp \
    Views/MemoryInfoView/MemoryInfoView.cpp \
    Views/OsInfoView/OsInfoView.cpp \
    Views/TimingInfoView/TimingInfoView.cpp \
    Views/TopologyNodeView/TopologyNodeView.cpp \
    Views/TopologyView/TopologyView.cpp \
    Main.cpp \
    MainWindow.cpp


HEADERS += \
	Dialogs/BenchmarkRunningDialog/BenchmarkRunningDialog.hpp \
	Views/BenchmarkRunConfigurationView/BenchmarkRunConfigurationView.hpp \
	Views/ConfigurationViews/ConfigurationView.hpp \
    Views/ConfigurationViews/FileConfigurationView/FileConfigurationView.hpp \
    Views/ConfigurationViews/FloatConfigurationView/FloatConfigurationView.hpp \
    Views/ConfigurationViews/IntegerConfigurationView/IntegerConfigurationView.hpp \
    Views/ConfigurationViews/StringConfigurationView/StringConfigurationView.hpp \
    Views/CpuInfoView/CpuInfoView.hpp \
    Views/CustomBenchmarkConfigurationView/CustomBenchmarkConfigurationView.hpp \
    Views/CustomBenchmarkResultsView/CustomBenchmarkResultsView.hpp \
    Views/CustomBenchmarkView/CustomBenchmarkView.hpp \
    Views/FullBenchmarkView/FullBenchmarkView.hpp \
    Views/MemoryInfoView/MemoryInfoView.hpp \
    Views/OsInfoView/OsInfoView.hpp \
    Views/TimingInfoView/TimingInfoView.hpp \
    Views/TopologyNodeView/TopologyNodeView.hpp \
    Views/TopologyView/TopologyView.hpp \
    MainWindow.hpp


FORMS += \
	Dialogs/BenchmarkRunningDialog/BenchmarkRunningDialog.ui \
	Views/BenchmarkRunConfigurationView/BenchmarkRunConfigurationView.ui \
	Views/ConfigurationViews/ConfigurationView.ui \
    Views/ConfigurationViews/FileConfigurationView/FileConfigurationView.ui \
    Views/ConfigurationViews/FloatConfigurationView/FloatConfigurationView.ui \
    Views/ConfigurationViews/IntegerConfigurationView/IntegerConfigurationView.ui \
    Views/ConfigurationViews/StringConfigurationView/StringConfigurationView.ui \
    Views/CpuInfoView/CpuInfoView.ui \
    Views/CustomBenchmarkConfigurationView/CustomBenchmarkConfigurationView.ui \
    Views/CustomBenchmarkResultsView/CustomBenchmarkResultsView.ui \
    Views/CustomBenchmarkView/CustomBenchmarkView.ui \
    Views/FullBenchmarkView/FullBenchmarkView.ui \
    Views/MemoryInfoView/MemoryInfoView.ui \
    Views/OsInfoView/OsInfoView.ui \
    Views/TimingInfoView/TimingInfoView.ui \
    Views/TopologyNodeView/TopologyNodeView.ui \
    Views/TopologyView/TopologyView.ui \
    MainWindow.ui

RESOURCES += \
    Images/Images.qrc

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include

DISTFILES +=


