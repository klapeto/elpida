//
// Created by klapeto on 25/4/20.
//

#ifndef APPS_QT_VIEWS_CONFIGURATIONVIEWS_TASKCONFIGURATIONLISTITEMVIEWBASE_HPP
#define APPS_QT_VIEWS_CONFIGURATIONVIEWS_TASKCONFIGURATIONLISTITEMVIEWBASE_HPP

#include <QListWidgetItem>

namespace Elpida
{
	class TaskConfiguration;

	class TaskConfigurationListItemViewBase : public QListWidgetItem
	{
	public:

		virtual void setTaskConfiguration(TaskConfiguration* configuration) = 0;
		virtual void saveSetting() = 0;

		TaskConfigurationListItemViewBase()
			: QListWidgetItem()
		{

		}
		~TaskConfigurationListItemViewBase() override = default;
	};
}


#endif //APPS_QT_VIEWS_CONFIGURATIONVIEWS_TASKCONFIGURATIONLISTITEMVIEWBASE_HPP
