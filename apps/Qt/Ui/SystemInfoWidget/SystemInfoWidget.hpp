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
		class SystemInfoWidget;
	}

	class SystemInfoWidget : public QWidget
	{
	Q_OBJECT

	public:
		explicit SystemInfoWidget(Mediator& mediator, const CpuInfo& cpuInfo, const SystemTopology& topology);
		~SystemInfoWidget() override;

	private:
		Ui::SystemInfoWidget* _ui;
		Mediator& _mediator;

		void loadCpuInfo(const CpuInfo& cpuInfo, const SystemTopology& topology);
	};

} // namespace Elpida
#endif // APPS_QT_UI_SYSTEMINFOWIDGET_SYSTEMINFOWIDGET_HPP
