#include "TimingInfoView.hpp"
#include "Elpida/Core/TimingInfo.hpp"
#include "ui_TimingInfoView.h"

#include "Models/TimingModel.hpp"
#include "Elpida/Core/ValueUtilities.hpp"
#include <string>

using Vu = Elpida::ValueUtilities;

namespace Elpida::Application
{
	static QString GetStabilityString(TimingStability stability)
	{
		switch (stability)
		{
		case TimingStability::ExtremelyUnstable:
			return "<b style=\"color: red\">Extremely unstable (0/10)</b>";
		case TimingStability::VeryUnstable:
			return "<b style=\"color: red\">Very unstable (2/10)</b>";
		case TimingStability::Unstable:
			return "<b style=\"color: red\">Unstable (4/10)</b>";
		case TimingStability::Stable:
			return "<b style=\"color: green\">Stable (6/10)</b>";
		case TimingStability::VeryStable:
			return "<b style=\"color: green\">Very Stable (8/10)</b>";
		case TimingStability::ExtremelyStable:
			return "<b style=\"color: green\">Extremely Stable (10/10)</b>";
		}
		return "<b style=\"color: green\">Extremely Stable</b>";
	}

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

		_ui->lblTimingStabilityValue->setText(GetStabilityString(model.GetTimingStability()));

		_ui->lblIpsValue->setText(QString::fromStdString(
			Vu::Cs(Vu::GetValueScaleStringSI(model.GetIterationsPerSecond()), "ips")));
	}

	TimingInfoView::~TimingInfoView()
	{
		delete _ui;
	}
}



