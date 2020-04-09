#ifndef ELPIDA_SYSTEMINFOWIDGET_HPP
#define ELPIDA_SYSTEMINFOWIDGET_HPP

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
		~SystemInfoWidget();

	private:
		Ui::SystemInfoWidget* _ui;
		Mediator& _mediator;

		void loadCpuInfo(const CpuInfo& cpuInfo, const SystemTopology& topology);
	};

} // namespace Elpida
#endif // ELPIDA_SYSTEMINFOWIDGET_HPP
