#include "TimingInfoView.hpp"
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

		_ui->lblStableTimeValue->setText(QString::fromStdString(
			Vu::Cs(Vu::GetValueScaleStringSI(model.GetStableTime().count()), "s")));

		_ui->lblSystemStableValue->setText(QString::fromStdString(
			Vu::Cs("<b>", model.IsSystemStable() ? "True": "False", "</b>")));

		_ui->lblSystemStableValue->setStyleSheet(model.IsSystemStable() ? "color: green" : "color: red");
	}

	TimingInfoView::~TimingInfoView()
	{
		delete _ui;
	}
}



