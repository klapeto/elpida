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
// Created by klapeto on 25/4/20.
//

#ifndef APPS_QT_VIEWS_CONFIGURATIONVIEWS_TASKCONFIGURATIONLISTITEMVIEWBASE_HPP
#define APPS_QT_VIEWS_CONFIGURATIONVIEWS_TASKCONFIGURATIONLISTITEMVIEWBASE_HPP

#include <QListWidgetItem>

#include <Elpida/Utilities/OptionalReference.hpp>
#include <Elpida/Engine/Configuration/Concrete/TaskConfiguration.hpp>

namespace Elpida
{
	class TaskConfiguration;

	class TaskConfigurationListItemViewBase : public QListWidgetItem
	{
	public:

		virtual void setTaskConfiguration(OptionalReference<TaskConfiguration> configuration) = 0;
		virtual void saveSetting() = 0;

		TaskConfigurationListItemViewBase()
			: QListWidgetItem()
		{

		}
		~TaskConfigurationListItemViewBase() override = default;
	};
}


#endif //APPS_QT_VIEWS_CONFIGURATIONVIEWS_TASKCONFIGURATIONLISTITEMVIEWBASE_HPP
