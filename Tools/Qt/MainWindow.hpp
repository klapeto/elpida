#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <Elpida/TaskBatchLoader.hpp>
#include <QMainWindow>

namespace Ui
{
	class MainWindow;

}
class TaskBatchProperties;

class MainWindow final: public QMainWindow
{
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	protected:
		virtual void showEvent(QShowEvent *event) override;

	private slots:
		void on_actionExit_triggered();
		void on_actionAbout_triggered();

	private:
		Elpida::TaskBatchLoader _tasksLoader;
		Ui::MainWindow *_ui;
		TaskBatchProperties* _taskBatchPropertiesDialog;
		bool _fixedSizeSet;

		void loadCpuInfo();
		void loadTaskInfo();
};

#endif // MAINWINDOW_HPP
