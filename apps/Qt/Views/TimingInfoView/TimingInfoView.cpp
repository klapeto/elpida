#include "TimingInfoView.hpp"
#include "Elpida/Core/TimingInfo.hpp"
#include "ui_TimingInfoView.h"

#include "Models/TimingModel.hpp"
#include "Elpida/Core/ValueUtilities.hpp"
#include <string>

using Vu = Elpida::ValueUtilities;

namespace Elpida::Application
{
	TimingInfoView::TimingInfoView(const TimingModel& model, QWidget* parent) :
		QWidget(parent),
		_ui(new Ui::TimingInfoView)
	{
		_ui->setupUi(this);
		_ui->lblNowOverheadValue->setText(
			QString::fromStdString(Vu::Cs(Vu::GetValueScaleStringSI(model.GetNowOverhead().count()), "s")));
		_ui->lblLoopOverheadValue->setText(QString::fromStdString(
			Vu::Cs(Vu::GetValueScaleStringSI(model.GetLoopOverhead().count()), "s")));
		_ui->lblVirtualOverheadValue->setText(QString::fromStdString(
			Vu::Cs(Vu::GetValueScaleStringSI(model.GetVirtualCallOverhead().count()), "s")));

		_ui->lblIpsValue->setText(QString::fromStdString(
			Vu::Cs(Vu::GetValueScaleStringSI(model.GetIterationsPerSecond()), "ips")));
	}

	TimingInfoView::~TimingInfoView()
	{
		delete _ui;
	}
}



