#ifndef ELPIDA_TASKRESULTSWIDGET_HPP
#define ELPIDA_TASKRESULTSWIDGET_HPP

#include <QWidget>
#include <Elpida/Engine/Result/BenchmarkResult.hpp>
#include "Models/Abstractions/CollectionModelObserver.hpp"

class QTreeWidgetItem;

namespace Elpida
{

	namespace Ui
	{
		class BenchmarkResultsView;
	}

	class BenchmarkResultsView : public QWidget, public CollectionModelObserver<BenchmarkResult>
	{
	Q_OBJECT

	public:
		explicit BenchmarkResultsView(const CollectionModel<BenchmarkResult>& model);
		~BenchmarkResultsView() override;
	private:
		Ui::BenchmarkResultsView* _ui;
		std::unordered_map<std::string, QTreeWidgetItem*> _createdItems;
	protected:
		void onItemAdded(const BenchmarkResult &item) override;
		void onItemRemoved(const BenchmarkResult &item) override;
		void onCollectionCleared() override;
	};

} // namespace Elpida
#endif // ELPIDA_TASKRESULTSWIDGET_HPP
