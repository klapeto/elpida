#include <QApplication>

#include "Tools/Qt/MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();

    return application.exec();
}
