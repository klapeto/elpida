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
        Ui/MainWindow/MainWindow.cpp \
    Ui/TaskBatchProperties/TaskBatchProperties.cpp \
    ElpidaManager.cpp \
    Ui/LogsDialog/LogsDialog.cpp \
    Ui/TaskBatchesWidget/TaskBatchesWidget.cpp \
    Ui/ListItemWithButton/ListItemWithButton.cpp \
    Ui/TopologyWidget/TopologyWidget.cpp \
    Ui/MascotWidget/MascotWidget.cpp \
    Ui/TopologyFrame/TopologyFrame.cpp

HEADERS += \
        Ui/MainWindow/MainWindow.hpp \
   Ui/TaskBatchProperties/TaskBatchProperties.hpp \
    ElpidaManager.hpp \
    Ui/LogsDialog/LogsDialog.hpp \
    Ui/TaskBatchesWidget/TaskBatchesWidget.hpp \
    Ui/ListItemWithButton/ListItemWithButton.hpp \
    Ui/TopologyWidget/TopologyWidget.hpp \
    Ui/MascotWidget/MascotWidget.hpp \
    Ui/TopologyFrame/TopologyFrame.hpp

FORMS += \
        Ui/MainWindow/MainWindow.ui \
    Ui/TaskBatchProperties/TaskBatchProperties.ui \
    Ui/LogsDialog/LogsDialog.ui \
    Ui/TaskBatchesWidget/TaskBatchesWidget.ui \
    Ui/ListItemWithButton/ListItemWithButton.ui \
    Ui/TopologyWidget/TopologyWidget.ui \
    Ui/TopologyFrame/TopologyFrame.ui

RESOURCES += \
    Images/Images.qrc

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include

DISTFILES +=


