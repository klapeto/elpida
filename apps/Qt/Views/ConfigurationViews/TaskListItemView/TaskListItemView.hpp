#ifndef ELPIDA_TASKLISTITEMVIEW_HPP
#define ELPIDA_TASKLISTITEMVIEW_HPP

#include "Views/ConfigurationViews/TaskConfigurationListItemViewBase.hpp"

namespace Elpida
{
	class TaskConfiguration;

	class TaskListItemView : public TaskConfigurationListItemViewBase
	{
	public:

		void setTaskConfiguration(TaskConfiguration* configuration) override;
		void saveSetting() override;

		explicit TaskListItemView();
		~TaskListItemView() override = default;
	private:
		TaskConfiguration* _taskConfiguration;
	};

} // namespace Elpida
#endif // ELPIDA_TASKLISTITEMVIEW_HPP
