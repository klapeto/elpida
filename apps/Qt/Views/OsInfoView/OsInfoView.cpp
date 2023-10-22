#include "OsInfoView.hpp"
#include "ui_OsInfoView.h"

#include "Models/SystemInfo/OsInfoModel.hpp"

namespace Elpida::Application
{
	OsInfoView::OsInfoView(const OsInfoModel& osInfo, QWidget* parent) :
			QWidget(parent),
			_ui(new Ui::OsInfoView)
	{
		_ui->setupUi(this);

		_ui->lblCategoryValue->setText(QString::fromStdString(osInfo.GetCategory()));
		_ui->lblNameValue->setText(QString::fromStdString(osInfo.GetName()));
		_ui->lblVersionValue->setText(QString::fromStdString(osInfo.GetVersion()));
	}

	OsInfoView::~OsInfoView()
	{
		delete _ui;
	}
}

