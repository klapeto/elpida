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
// Created by klapeto on 11/9/2023.
//

#ifndef ELPIDA_APPS_APPLICATION_CORE_SETTINGSSERVICE_HPP
#define ELPIDA_APPS_APPLICATION_CORE_SETTINGSSERVICE_HPP

#include <string>

namespace Elpida::Application
{
	class SettingsService
	{
	public:
		virtual void Set(const std::string& name, const std::string& value) = 0;
		virtual std::string Get(const std::string& name) = 0;
	};

} // Application

#endif //ELPIDA_APPS_APPLICATION_CORE_SETTINGSSERVICE_HPP
