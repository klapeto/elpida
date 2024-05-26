//
// Created by klapeto on 26/5/2024.
//

#include "QtPathsService.hpp"

#include <QStandardPaths>

namespace Elpida::Application
{
	const std::filesystem::path& QtPathsService::GetDownloadStoragePath() const
	{
		return _downloadPath;
	}

	QtPathsService::QtPathsService()
	{
		_downloadPath = std::filesystem::path(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation).toStdString());
	}

} // Application
// Elpida