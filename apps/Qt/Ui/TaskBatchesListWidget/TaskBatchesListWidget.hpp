#ifndef APPS_QT_UI_TASKBATCHESLISTWIDGET_TASKBATCHESLISTWIDGET_HPP
#define APPS_QT_UI_TASKBATCHESLISTWIDGET_TASKBATCHESLISTWIDGET_HPP

#include <QWidget>
#include "Models/Abstractions/CollectionItem.hpp"
#include "Models/Abstractions/EventArgs/CollectionChangedEventArgs.hpp"
#include "Elpida/EventSubscription.hpp"

#include <unordered_map>

class QListWidgetItem;

namespace Elpida
{
	template <typename T>
	class CollectionModel;

	class QtTaskBatchWrapper;
	class EventSubscriptionBase;

	template <typename T>
	class CollectionChangedEventArgs;

	namespace Ui
	{
		class TaskBatchesListWidget;
	}

	class TaskBatchesListWidget : public QWidget
	{
		Q_OBJECT

	public:

		QtTaskBatchWrapper* getSelectedTaskBatch();

		explicit TaskBatchesListWidget(const CollectionModel<QtTaskBatchWrapper*>& model);
		~TaskBatchesListWidget() override;
	private:
		Ui::TaskBatchesListWidget* _ui;

		std::vector<EventSubscriptionBase*> _subscriptions;
		std::unordered_map<const QtTaskBatchWrapper*, QListWidgetItem*> _createdItems;
		const CollectionModel<QtTaskBatchWrapper*>& _model;

		void onItemRemoved(const CollectionChangedEventArgs<QtTaskBatchWrapper*>& item);
		void onItemAdded(const CollectionChangedEventArgs<QtTaskBatchWrapper*>& item);
		void onCleared();
	};

} // namespace Elpida
#endif // APPS_QT_UI_TASKBATCHESLISTWIDGET_TASKBATCHESLISTWIDGET_HPP
