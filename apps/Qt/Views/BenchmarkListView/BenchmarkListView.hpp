#ifndef APPS_QT_UI_TASKBATCHESLISTWIDGET_BENCHMARKLISTVIEW_HPP
#define APPS_QT_UI_TASKBATCHESLISTWIDGET_BENCHMARKLISTVIEW_HPP

#include <QWidget>
#include "Models/Abstractions/CollectionModelObserver.hpp"
#include "Models/Abstractions/ListModel/ListModel.hpp"
#include "Core/Abstractions/CommandHandler.hpp"

#include <unordered_map>

class QListWidgetItem;

namespace Elpida
{
	class Benchmark;

	namespace Ui
	{
		class BenchmarkListView;
	}

	class BenchmarkListView : public QWidget, public CollectionModelObserver<Benchmark*>, public CommandHandler
	{
	Q_OBJECT

	public:

		void handle(GetBenchmarksToRunCommand &command) override;

		explicit BenchmarkListView(const ListModel<Benchmark*>& model);
		~BenchmarkListView() override;
	private:
		Ui::BenchmarkListView* _ui;
		std::unordered_map<const Benchmark*, QListWidgetItem*> _createdItems;

		Benchmark* getSelectedBenchmark();
	protected:

		void onItemAdded(Benchmark* const& item) override;
		void onItemRemoved(Benchmark* const& item) override;
		void onCollectionCleared() override;
		void addItem(Benchmark* const& item);
	};

} // namespace Elpida
#endif // APPS_QT_UI_TASKBATCHESLISTWIDGET_BENCHMARKLISTVIEW_HPP
