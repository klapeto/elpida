#ifndef ELPIDA_TASKRESULTSWIDGET_HPP
#define ELPIDA_TASKRESULTSWIDGET_HPP

#include <QWidget>
#include <Elpida/TaskBatchRunResult.hpp>
#include "Models/Abstractions/CollectionModelObserver.hpp"

class QTreeWidgetItem;

namespace Elpida
{

	namespace Ui
	{
		class TaskResultsWidget;
	}

	class TaskResultsWidget : public QWidget, public CollectionModelObserver<TaskBatchRunResult>
	{
	Q_OBJECT

	public:
		explicit TaskResultsWidget(const CollectionModel<TaskBatchRunResult>& model);
		~TaskResultsWidget() override;
	private:
		Ui::TaskResultsWidget* _ui;
		std::unordered_map<std::string, QTreeWidgetItem*> _createdItems;
	protected:
		void onItemAdded(const TaskBatchRunResult &item) override;
		void onItemRemoved(const TaskBatchRunResult &item) override;
		void onCollectionCleared() override;
	};

} // namespace Elpida
#endif // ELPIDA_TASKRESULTSWIDGET_HPP
