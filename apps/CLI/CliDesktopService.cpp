/*
 *  Copyright (c) 2024-2025  Ioannis Panagiotopoulos
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
// Created by klapeto on 30/9/2024.
//

#include "CliDesktopService.hpp"
#include <iostream>

namespace Elpida::Application
{
	void CliDesktopService::OpenUri(const std::string& uri) const
	{
		if (!_quiet)
		{
			std::cout << "Posted on URI: " << uri << std::endl;
		}
	}

	void CliDesktopService::OpenFile(const std::filesystem::path& path) const
	{
		if (!_quiet)
		{
			std::cout << "Stored on path: " << path << std::endl;
		}
	}

	CliDesktopService::CliDesktopService(bool quiet)
			:_quiet(quiet)
	{
	}
} // Application
// Elpida