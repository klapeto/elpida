#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Ui
{
	class MainWindow;
}

namespace Elpida::Application
{
	class OsInfoModel;
	class MemoryInfoModel;
	class CpuInfoModel;
	class OverheadsModel;

	class MainWindow : public QMainWindow
	{
	Q_OBJECT
	public:
		explicit MainWindow(
				const OsInfoModel& osInfo,
				const MemoryInfoModel& memoryInfo,
				const CpuInfoModel& cpuInfo,
				const OverheadsModel& overheadsInfo,
				QWidget* parent = nullptr);

		~MainWindow() override;

	private:
		Ui::MainWindow* _ui;
	};
}
#endif // MAINWINDOW_HPP
