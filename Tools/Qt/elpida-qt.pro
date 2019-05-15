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
        MainWindow.cpp \
    TaskBatchProperties.cpp \
    StaticAspectRatioWidget.cpp \
    ElpidaManager.cpp \
    LogsDialog.cpp \
    TaskBatchesWidget.cpp \
    ListItemWithButton.cpp \
    TopologyWidget.cpp

HEADERS += \
        MainWindow.hpp \
    TaskBatchProperties.hpp \
    ElpidaManager.hpp \
    LogsDialog.hpp \
    TaskBatchesWidget.hpp \
    ListItemWithButton.hpp \
    TopologyWidget.hpp

FORMS += \
        MainWindow.ui \
    TaskBatchProperties.ui \ 
    LogsDialog.ui \
    TaskBatchesWidget.ui \
    ListItemWithButton.ui \
    TopologyWidget.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../elpida/install/lib64/release/ -lelpida
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../elpida/install/lib64/debug/ -lelpida
else:unix: LIBS += -L$$PWD/../elpida/install/lib64/ -lelpida

INCLUDEPATH += $$PWD/../elpida
DEPENDPATH += $$PWD/../elpida/Elpida

DISTFILES +=

RESOURCES += \
    Images.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../install/lib64/release/ -lelpida
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../install/lib64/debug/ -lelpida
else:unix: LIBS += -L$$PWD/../../install/lib64/ -lelpida

INCLUDEPATH += $$PWD/../..
DEPENDPATH += $$PWD/../..
