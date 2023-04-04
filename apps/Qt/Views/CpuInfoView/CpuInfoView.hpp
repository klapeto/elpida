#ifndef ELPIDA_CPUINFOVIEW_HPP
#define ELPIDA_CPUINFOVIEW_HPP

#include <QWidget>

namespace Elpida::Application
{
	namespace Ui
	{
		class CpuInfoView;
	}

	class CpuInfoModel;

	class CpuInfoView : public QWidget
	{
	Q_OBJECT

	public:
		explicit CpuInfoView(const CpuInfoModel& cpuInfo, QWidget* parent = nullptr);
		~CpuInfoView() override;
	private:
		Ui::CpuInfoView* _ui;
	};
}

#endif // ELPIDA_CPUINFOVIEW_HPP
