//
// Created by klapeto on 26/5/2024.
//

#include "QtPathsService.hpp"
#include "Elpida/Platform/OsUtilities.hpp"

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
		_executablePath = OsUtilities::GetExecutableDirectory();
	}

	const std::filesystem::path& QtPathsService::GetExecutablePath() const
	{
		return _executablePath;
	}

	std::filesystem::path QtPathsService::GetFullReportOutputPath() const
	{
		std::string fileName = "Full Benchmark ";
		fileName
				.append(" ")
				.append(std::to_string(time(nullptr)))
				.append(".html");

		return GetDownloadStoragePath() / "Elpida Exported Reports" / fileName;
	}

} // Application
// Elpida