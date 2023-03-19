#ifndef ELPIDA_CPUINFOVIEW_HPP
#define ELPIDA_CPUINFOVIEW_HPP

#include <QWidget>

namespace Ui
{
	class CpuInfoView;
}

namespace Elpida
{
	class CpuInfo;
}

class CpuInfoView : public QWidget
{
Q_OBJECT

public:
	explicit CpuInfoView(const Elpida::CpuInfo& cpuInfo, QWidget* parent = nullptr);

	~CpuInfoView() override;

private:
	Ui::CpuInfoView* _ui;
};

#endif // ELPIDA_CPUINFOVIEW_HPP
