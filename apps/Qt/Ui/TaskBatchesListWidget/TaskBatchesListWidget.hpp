#ifndef APPS_QT_UI_TASKBATCHESLISTWIDGET_TASKBATCHESLISTWIDGET_HPP
#define APPS_QT_UI_TASKBATCHESLISTWIDGET_TASKBATCHESLISTWIDGET_HPP

#include <QWidget>
#include "Models/Abstractions/CollectionModelObserver.hpp"
#include "Core/Abstractions/CommandHandler.hpp"

#include <unordered_map>

class QListWidgetItem;

namespace Elpida
{
	class Benchmark;
	class TaskBatch;

	namespace Ui
	{
		class TaskBatchesListWidget;
	}

	class TaskBatchesListWidget : public QWidget, public CollectionModelObserver<Benchmark*>, public CommandHandler
	{
	Q_OBJECT

	public:

		void handle(GetSelectedTaskBatchesCommand &command) override;

		TaskBatch* getSelectedTaskBatch();

		explicit TaskBatchesListWidget(const CollectionModel<Benchmark*>& model);
		~TaskBatchesListWidget() override;
	private:
		Ui::TaskBatchesListWidget* _ui;
		std::unordered_map<const Benchmark*, QListWidgetItem*> _createdItems;
	protected:

		void onItemAdded(Benchmark* const& item) override;
		void onItemRemoved(Benchmark* const& item) override;
		void onCollectionCleared() override;
		void addItem(Benchmark* const& item);
	};

} // namespace Elpida
#endif // APPS_QT_UI_TASKBATCHESLISTWIDGET_TASKBATCHESLISTWIDGET_HPP
