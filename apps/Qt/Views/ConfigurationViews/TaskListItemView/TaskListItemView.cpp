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

#include "TaskListItemView.hpp"

#include <Elpida/Engine/Configuration/Concrete/TaskConfiguration.hpp>
#include <Elpida/Engine/Task/TaskBuilder.hpp>
#include <Elpida/Engine/Task/TaskSpecification.hpp>
#include <QCheckBox>

namespace Elpida
{

	TaskListItemView::TaskListItemView()
		: TaskConfigurationListItemViewBase(),
		_taskConfiguration(std::nullopt)
	{
		setCheckState(Qt::Checked);
	}

	void TaskListItemView::setTaskConfiguration(OptionalReference<TaskConfiguration> configuration)
	{
		_taskConfiguration = configuration;
		if (_taskConfiguration.has_value())
		{
			if (_taskConfiguration->get().getTaskBuilder().canBeDisabled())
			{
				setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
				setCheckState(_taskConfiguration->get().isEnabled() ? Qt::Checked : Qt::Unchecked);
			}
			else
			{
				setCheckState(Qt::Checked);
				setFlags(Qt::NoItemFlags);
			}

			setText(QString::fromStdString(_taskConfiguration->get().getTaskSpecification().getName()));
		}
		else
		{
			setCheckState(Qt::Checked);
			setFlags(Qt::NoItemFlags);
		}
	}

	void TaskListItemView::saveSetting()
	{
		if (_taskConfiguration.has_value())
		{
			if (_taskConfiguration->get().getTaskBuilder().canBeDisabled())
			{
				_taskConfiguration->get().setEnabled(checkState() == Qt::Checked);
			}
		}
	}

} // namespace Elpida
