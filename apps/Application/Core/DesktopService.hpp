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

#ifndef ELPIDA_DESKTOPSERVICE_HPP
#define ELPIDA_DESKTOPSERVICE_HPP

#include <string>
#include <filesystem>

namespace Elpida::Application
{

	class DesktopService
	{
	public:
		virtual void OpenUri(const std::string& uri) const = 0;
		virtual void OpenFile(const std::filesystem::path& path) const = 0;

		DesktopService() = default;
		virtual ~DesktopService() = default;
	};

} // Application
// Elpida

#endif //ELPIDA_DESKTOPSERVICE_HPP
