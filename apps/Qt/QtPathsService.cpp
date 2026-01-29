/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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