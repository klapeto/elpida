#ifndef APPS_QT_UI_SYSTEMINFOWIDGET_SYSTEMINFOWIDGET_HPP
#define APPS_QT_UI_SYSTEMINFOWIDGET_SYSTEMINFOWIDGET_HPP

#include <QWidget>

namespace Elpida
{

	class CpuInfo;
	class SystemTopology;
	class Mediator;

	namespace Ui
	{
		class SystemInfoView;
	}

	class SystemInfoView : public QWidget
	{
	Q_OBJECT

	public:
		explicit SystemInfoView(const CpuInfo& cpuInfo, const SystemTopology& topology);
		~SystemInfoView() override;

	private:
		Ui::SystemInfoView* _ui;

		void loadCpuInfo(const CpuInfo& cpuInfo, const SystemTopology& topology);
	};

} // namespace Elpida
#endif // APPS_QT_UI_SYSTEMINFOWIDGET_SYSTEMINFOWIDGET_HPP
