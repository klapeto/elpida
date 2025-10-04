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

#ifndef ELPIDA_APPS_QT_QTSETTINGSSERVICE_HPP
#define ELPIDA_APPS_QT_QTSETTINGSSERVICE_HPP

#include <QSettings>
#include "Core/SettingsService.hpp"

namespace Elpida::Application
{

	class QtSettingsService final: public SettingsService
	{
	public:
		void Set(const std::string& name, const std::string& value) override;
		std::string Get(const std::string& name) override;

		QtSettingsService() = default;
		QtSettingsService(const QtSettingsService&) = delete;
		QtSettingsService(QtSettingsService&&) noexcept= delete;
		QtSettingsService operator=(const QtSettingsService&) = delete;
		QtSettingsService operator=(const QtSettingsService&&) noexcept = delete;
		~QtSettingsService() = default;
	private:
		QSettings _settings;
	};

} // Application

#endif //ELPIDA_APPS_QT_QTSETTINGSSERVICE_HPP
