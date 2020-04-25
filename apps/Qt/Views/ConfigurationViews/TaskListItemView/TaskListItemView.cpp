#include "TaskListItemView.hpp"
//#include "ui_TaskListItemView.h"

#include <Elpida/Engine/Configuration/TaskConfiguration.hpp>
#include <Elpida/Engine/Task/TaskSpecification.hpp>
#include <QCheckBox>

namespace Elpida
{

	TaskListItemView::TaskListItemView()
		: TaskConfigurationListItemViewBase(), _taskConfiguration(nullptr)
	{
		setCheckState(Qt::Checked);
	}

	void TaskListItemView::setTaskConfiguration(TaskConfiguration* taskConfiguration)
	{
		_taskConfiguration = taskConfiguration;
		if (taskConfiguration != nullptr)
		{
			if (taskConfiguration->getTaskSpecification().canBeDisabled())
			{
				setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
				setCheckState(taskConfiguration->isEnabled() ? Qt::Checked : Qt::Unchecked);
			}
			else
			{
				setCheckState(Qt::Checked);
				setFlags(Qt::NoItemFlags);
			}

			setText(QString::fromStdString(taskConfiguration->getTaskSpecification().getName()));
		}
		else
		{
			setCheckState(Qt::Checked);
			setFlags(Qt::NoItemFlags);
		}
	}

	void TaskListItemView::saveSetting()
	{
		if (_taskConfiguration != nullptr)
		{
			if (_taskConfiguration->getTaskSpecification().canBeDisabled())
			{
				_taskConfiguration->setEnabled(checkState() == Qt::Checked);
			}
		}
	}

} // namespace Elpida
