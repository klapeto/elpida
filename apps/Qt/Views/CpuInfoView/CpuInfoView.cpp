#include "CpuInfoView.hpp"
#include "ui_CpuInfoView.h"

#include "Layouts/FlowLayout.hpp"

#include "Elpida/Core/ValueUtilities.hpp"
#include "Models/SystemInfo/CpuInfoModel.hpp"

using Vu = Elpida::ValueUtilities;

namespace Elpida::Application
{
	CpuInfoView::CpuInfoView(const CpuInfoModel& cpuInfo, QWidget* parent) :
			QWidget(parent),
			_ui(new Ui::CpuInfoView)
	{
		_ui->setupUi(this);
		_ui->lblArchitectureValue->setText(QString::fromStdString(cpuInfo.GetArchitecture()));
		_ui->lblVendorValue->setText(QString::fromStdString(cpuInfo.GetVendorName()));
		_ui->lblModelNameValue->setText(QString::fromStdString(cpuInfo.GetModelName()));
	}

	CpuInfoView::~CpuInfoView()
	{
		delete _ui;
	}
}