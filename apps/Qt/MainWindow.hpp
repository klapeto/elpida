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
}

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
	explicit MainWindow(
			const Elpida::OsInfo& osInfo,
			const Elpida::MemoryInfo& memoryInfo,
			const Elpida::CpuInfo& cpuInfo,
			const Elpida::OverheadsInfo& overheadsInfo,
			QWidget* parent = nullptr);

	~MainWindow() override;

private:
	Ui::MainWindow* _ui;
};

#endif // MAINWINDOW_HPP
