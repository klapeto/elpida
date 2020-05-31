/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 22/4/20.
//

#ifndef APPS_QT_VIEWS_CONFIGURATIONVIEWS_CONFIGURATIONVALUEVIEWBASE_HPP
#define APPS_QT_VIEWS_CONFIGURATIONVIEWS_CONFIGURATIONVALUEVIEWBASE_HPP

#include <QWidget>

namespace Elpida
{
	class ConfigurationValueBase;

	class ConfigurationValueViewBase : public QWidget
	{
	public:
		virtual void setConfiguration(ConfigurationValueBase* configurationValue) = 0;
		virtual ConfigurationValueBase* getConfiguration() = 0;
		virtual void saveSetting() = 0;

		ConfigurationValueViewBase()
			: QWidget()
		{

		}
		~ConfigurationValueViewBase() override = default;
	};
}

#endif //APPS_QT_VIEWS_CONFIGURATIONVIEWS_CONFIGURATIONVALUEVIEWBASE_HPP
