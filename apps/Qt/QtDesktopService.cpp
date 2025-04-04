//
// Created by klapeto on 26/5/2024.
//

#include "QtDesktopService.hpp"

#include <QDesktopServices>
#include <QUrl>

namespace Elpida::Application
{
	void QtDesktopService::OpenUri(const std::string& uri) const
	{
		QDesktopServices::openUrl(QUrl(QString::fromStdString(uri)));
	}

	void QtDesktopService::OpenFile(const std::filesystem::path& path) const
	{
		QDesktopServices::openUrl(QUrl::fromLocalFile(QString::fromStdString(path.generic_string())));
	}
} // Application
// Elpida