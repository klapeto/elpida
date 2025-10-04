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

#include "QtSettingsService.hpp"

namespace Elpida::Application
{
	void QtSettingsService::Set(const std::string& name, const std::string& value)
	{
		_settings.setValue(QString::fromStdString(name), QString::fromStdString(value));
	}

	std::string QtSettingsService::Get(const std::string& name)
	{
		return _settings.value(QString::fromStdString(name)).toString().toStdString();
	}
} // Application