#ifndef APPS_QT_UI_TASKBATCHESLISTWIDGET_TASKBATCHESLISTWIDGET_HPP
#define APPS_QT_UI_TASKBATCHESLISTWIDGET_TASKBATCHESLISTWIDGET_HPP

#include <QWidget>
#include "Models/Abstractions/CollectionModelObserver.hpp"
#include "Core/Abstractions/CommandHandler.hpp"

#include <unordered_map>

class QListWidgetItem;

namespace Elpida
{
	class QtTaskBatchWrapper;

	namespace Ui
	{
		class TaskBatchesListWidget;
	}

	class TaskBatchesListWidget : public QWidget, public CollectionModelObserver<QtTaskBatchWrapper*>, public CommandHandler
	{
	Q_OBJECT

	public:

		void handle(GetSelectedTaskBatchesCommand &command) override;

		QtTaskBatchWrapper* getSelectedTaskBatch();

		explicit TaskBatchesListWidget(const CollectionModel<QtTaskBatchWrapper*>& model);
		~TaskBatchesListWidget() override;
	private:
		Ui::TaskBatchesListWidget* _ui;
		std::unordered_map<const QtTaskBatchWrapper*, QListWidgetItem*> _createdItems;
	protected:

		void onItemAdded(QtTaskBatchWrapper* const& item) override;
		void onItemRemoved(QtTaskBatchWrapper* const& item) override;
		void onCollectionCleared() override;
	};

} // namespace Elpida
#endif // APPS_QT_UI_TASKBATCHESLISTWIDGET_TASKBATCHESLISTWIDGET_HPP
