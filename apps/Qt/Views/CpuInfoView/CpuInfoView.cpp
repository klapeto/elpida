#include "CpuInfoView.hpp"
#include "ui_CpuInfoView.h"

#include "Layouts/FlowLayout.hpp"

#include <QLabel>

#include "Elpida/Core/ValueUtilities.hpp"
#include "Models/CpuInfoModel.hpp"

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

//		auto additionalInfoLayout = new QGridLayout();
//
//		auto row = 0;
//		for (auto& info: cpuInfo.GetAdditionalInfo())
//		{
//			additionalInfoLayout->addWidget(new QLabel(QString::fromStdString(Vu::Cs(info.first, ':'))), row, 0);
//			additionalInfoLayout->addWidget(new QLabel(QString::fromStdString(info.second)), row, 1);
//			row++;
//		}
//		_ui->gnAdditionalInfo->setLayout(additionalInfoLayout);


		auto featuresLayout = new FlowLayout();
		for (auto& feature: cpuInfo.GetFeatures())
		{
			featuresLayout->addWidget(new QLabel(QString::fromStdString(feature)));
		}
		_ui->saFeatures->setLayout(featuresLayout);

		_ui->lblFrequencyValue->setText(QString::fromStdString(ValueUtilities::GetValueScaleStringSI(cpuInfo.GetFrequency()) + "Hz"));
	}

	CpuInfoView::~CpuInfoView()
	{
		delete _ui;
	}
}