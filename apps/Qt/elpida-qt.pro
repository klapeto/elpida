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
    Ui/CommonDialog/CommonDialog.cpp \
        Ui/MainWindow/MainWindow.cpp \
    Ui/SystemInfoWidget/SystemInfoWidget.cpp \
    Ui/TaskBatchProperties/TaskBatchProperties.cpp \
    ElpidaManager.cpp \
    Ui/LogsWidget/LogsWidget.cpp \
    Ui/TaskBatchRunnerStatusView/TaskBatchRunnerStatusView.cpp \
    Ui/TaskBatchesListWidget/TaskBatchesListWidget.cpp \
    Ui/TaskBatchesWidget/TaskBatchesWidget.cpp \
    Ui/TaskResultsWidget/TaskResultsWidget.cpp \
    Ui/TopologyWidget/TopologyWidget.cpp \
    Ui/MascotWidget/MascotWidget.cpp \
    Ui/TopologyFrame/TopologyFrame.cpp

HEADERS += \
    Ui/CommonDialog/CommonDialog.hpp \
        Ui/MainWindow/MainWindow.hpp \
    Ui/SystemInfoWidget/SystemInfoWidget.hpp \
   Ui/TaskBatchProperties/TaskBatchProperties.hpp \
    ElpidaManager.hpp \
    Ui/LogsWidget/LogsWidget.hpp \
    Ui/TaskBatchRunnerStatusView/TaskBatchRunnerStatusView.hpp \
    Ui/TaskBatchesListWidget/TaskBatchesListWidget.hpp \
    Ui/TaskBatchesWidget/TaskBatchesWidget.hpp \
    Ui/TaskResultsWidget/TaskResultsWidget.hpp \
    Ui/TopologyWidget/TopologyWidget.hpp \
    Ui/MascotWidget/MascotWidget.hpp \
    Ui/TopologyFrame/TopologyFrame.hpp

FORMS += \
    Ui/CommonDialog/CommonDialog.ui \
        Ui/MainWindow/MainWindow.ui \
    Ui/SystemInfoWidget/SystemInfoWidget.ui \
    Ui/TaskBatchProperties/TaskBatchProperties.ui \
    Ui/LogsWidget/LogsWidget.ui \
    Ui/TaskBatchRunnerStatusView/TaskBatchRunnerStatusView.ui \
    Ui/TaskBatchesListWidget/TaskBatchesListWidget.ui \
    Ui/TaskBatchesWidget/TaskBatchesWidget.ui \
    Ui/TaskResultsWidget/TaskResultsWidget.ui \
    Ui/TopologyWidget/TopologyWidget.ui \
    Ui/TopologyFrame/TopologyFrame.ui

RESOURCES += \
    Images/Images.qrc

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include

DISTFILES +=


