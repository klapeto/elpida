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

#ifndef ELPIDA_TASKLISTITEMVIEW_HPP
#define ELPIDA_TASKLISTITEMVIEW_HPP

#include "Views/ConfigurationViews/TaskConfigurationListItemViewBase.hpp"

namespace Elpida
{
	class TaskConfiguration;

	class TaskListItemView : public TaskConfigurationListItemViewBase
	{
	public:

		void setTaskConfiguration(OptionalReference<TaskConfiguration> configuration) override;
		void saveSetting() override;

		explicit TaskListItemView();
		~TaskListItemView() override = default;
	private:
		OptionalReference<TaskConfiguration> _taskConfiguration;
	};

} // namespace Elpida

#endif // ELPIDA_TASKLISTITEMVIEW_HPP
