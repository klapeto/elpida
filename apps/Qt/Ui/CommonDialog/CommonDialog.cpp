#include "CommonDialog.hpp"
#include "ui_CommonDialog.h"

namespace Elpida
{

	CommonDialog::CommonDialog(QWidget* parent)
		:
		QDialog(parent),
		_ui(new Ui::CommonDialog)
	{
		_ui->setupUi(this);
	}

	CommonDialog::~CommonDialog()
	{
		delete _ui;
	}

	CommonDialog::DialogCode CommonDialog::show(QWidget* widget)
	{
		if (widget != nullptr)
		{
			_ui->verticalLayout->insertWidget(0, widget);
			auto result = (DialogCode)exec();
			_ui->verticalLayout->removeWidget(widget);
			return result;
		}
		return DialogCode::Rejected;
	}

} // namespace Elpida
