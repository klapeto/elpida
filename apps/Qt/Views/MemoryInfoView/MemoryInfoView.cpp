#include "MemoryInfoView.hpp"
#include "ui_MemoryInfoView.h"

#include "Models/MemoryInfoModel.hpp"
#include "Elpida/Core/ValueUtilities.hpp"

using Vu = Elpida::ValueUtilities;

namespace Elpida::Application
{
	MemoryInfoView::MemoryInfoView(const MemoryInfoModel& memoryInfo, QWidget* parent) :
			QWidget(parent),
			_ui(new Ui::MemoryInfoView)
	{
		_ui->setupUi(this);

		_ui->lblTotalSizeValue->setText(
				QString::fromStdString(Vu::Cs(Vu::GetValueScaleStringIEC(memoryInfo.GetTotalSize()), "B")));
		_ui->lblPageSizeValue->setText(
				QString::fromStdString(Vu::Cs(Vu::GetValueScaleStringIEC(memoryInfo.GetPageSize()), "B")));
	}

	MemoryInfoView::~MemoryInfoView()
	{
		delete _ui;
	}
}