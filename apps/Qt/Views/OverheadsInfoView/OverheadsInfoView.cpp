#include "OverheadsInfoView.hpp"
#include "ui_OverheadsInfoView.h"

#include "Elpida/Core/OverheadsInfo.hpp"
#include "Elpida/Core/ValueUtilities.hpp"

using Vu = Elpida::ValueUtilities;

OverheadsInfoView::OverheadsInfoView(const Elpida::OverheadsInfo& overheadsInfo, QWidget *parent) :
		QWidget(parent),
		_ui(new Ui::OverheadsInfoView)
{
	_ui->setupUi(this);
	_ui->lblNowOverheadValue->setText(QString::fromStdString(Vu::Cs(Vu::GetValueScaleStringSI(overheadsInfo.GetNowOverhead().count()),"s")));
	_ui->lblLoopOverheadValue->setText(QString::fromStdString(Vu::Cs(Vu::GetValueScaleStringSI(overheadsInfo.GetLoopOverhead().count()),"s")));
	_ui->lblVirtualOverheadValue->setText(QString::fromStdString(Vu::Cs(Vu::GetValueScaleStringSI(overheadsInfo.GetVirtualCallOverhead().count()),"s")));
}

OverheadsInfoView::~OverheadsInfoView()
{
	delete _ui;
}
