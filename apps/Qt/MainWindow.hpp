#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Elpida::Application
{
	namespace Ui
	{
		class MainWindow;
	}

	class OsInfoModel;
	class MemoryInfoModel;
	class CpuInfoModel;
	class OverheadsModel;
	class TopologyModel;

	class MainWindow : public QMainWindow
	{
	Q_OBJECT
	public:
		explicit MainWindow(
				const OsInfoModel& osInfo,
				const MemoryInfoModel& memoryInfo,
				const CpuInfoModel& cpuInfo,
				const OverheadsModel& overheadsInfo,
				TopologyModel& topologyModel,
				QWidget* parent = nullptr);

		~MainWindow() override;

	private:
		Ui::MainWindow* _ui;
	};
}
#endif // MAINWINDOW_HPP
