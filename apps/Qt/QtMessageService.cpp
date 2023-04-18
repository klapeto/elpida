//
// Created by klapeto on 17/4/2023.
//

#include "QtMessageService.hpp"

#include <QMessageBox>
#include <QApplication>

namespace Elpida::Application
{
	void QtMessageService::ShowMessage(const std::string& message)
	{
		QMessageBox::information(QApplication::activeWindow(), "Information", QString::fromStdString(message));
	}
	void QtMessageService::ShowWarning(const std::string& message)
	{
		QMessageBox::warning(QApplication::activeWindow(), "Warning", QString::fromStdString(message));
	}
	void QtMessageService::ShowError(const std::string& message)
	{
		QMessageBox::critical(QApplication::activeWindow(), "Error", QString::fromStdString(message));
	}
} // Application