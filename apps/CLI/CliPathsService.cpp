//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 30/9/2024.
//

#include "CliPathsService.hpp"

#include <utility>
#include "Elpida/Platform/OsUtilities.hpp"
#include "Elpida/Core/ElpidaException.hpp"


namespace Elpida::Application
{
	const std::filesystem::path& CliPathsService::GetDownloadStoragePath() const
	{
		return _download;
	}

	std::filesystem::path CliPathsService::GetFullReportOutputPath() const
	{
		return _reportPath;
	}

	const std::filesystem::path& CliPathsService::GetExecutablePath() const
	{
		return _executablePath;
	}

	CliPathsService::CliPathsService(std::filesystem::path reportPath)
			:_reportPath(std::move(reportPath))
	{
		_executablePath = OsUtilities::GetExecutableDirectory();
		_download = std::filesystem::current_path();
		if (is_directory(_reportPath))
		{
			throw ElpidaException("Path: ", _reportPath, " is a directory");
		}
	}
} // Application
// Elpida