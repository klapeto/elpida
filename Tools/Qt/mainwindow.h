#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    private slots:
        void on_actionExit_triggered();

        void on_actionAbout_triggered();

    private:
    Ui::MainWindow *ui;
    void loadCpuInfo();
    void loadTaskInfo();
};

#endif // MAINWINDOW_HPP
