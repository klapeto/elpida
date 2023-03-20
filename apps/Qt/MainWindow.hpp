#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Ui
{
	class MainWindow;
}

namespace Elpida
{
	class OsInfo;
	class MemoryInfo;
	class OverheadsInfo;
	class CpuInfo;

	namespace Application
	{
		class MainWindow : public QMainWindow
		{
		Q_OBJECT
		public:
			explicit MainWindow(
					const OsInfo& osInfo,
					const MemoryInfo& memoryInfo,
					const CpuInfo& cpuInfo,
					const OverheadsInfo& overheadsInfo,
					QWidget* parent = nullptr);

			~MainWindow() override;

		private:
			Ui::MainWindow* _ui;
		};
	}


}
#endif // MAINWINDOW_HPP
