#include "LogsDialog.hpp"
#include "ui_LogsDialog.h"

namespace Elpida {

	LogsDialog::LogsDialog(QWidget *parent) :
		QDialog(parent),
		_ui(new Ui::LogsDialog)
	{
		_ui->setupUi(this);
	}

	void LogsDialog::setLogsText(const String& text)
	{
		_ui->tbLogs->setText(QString::fromStdString(text));
	}

	LogsDialog::~LogsDialog()
	{
		delete _ui;
	}

} // namespace Elpida
