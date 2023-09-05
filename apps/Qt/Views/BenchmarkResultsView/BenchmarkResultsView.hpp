#ifndef ELPIDA_APPLICATION_BENCHMARKRESULTSVIEW_HPP
#define ELPIDA_APPLICATION_BENCHMARKRESULTSVIEW_HPP

#include "EventSubscription.hpp"
#include "Models/Abstractions/CollectionItem.hpp"
#include "Models/BenchmarkResultModel.hpp"
#include <QWidget>

namespace Elpida::Application
{

	class BenchmarkResultsModel;

	namespace Ui
	{
		class BenchmarkResultsView;
	}

	class BenchmarkResultsView : public QWidget
	{
	 Q_OBJECT

	 public:
		explicit BenchmarkResultsView(const BenchmarkResultsModel& model, QWidget* parent = nullptr);
		~BenchmarkResultsView() override;

	 private:
		Ui::BenchmarkResultsView* _ui;
		const BenchmarkResultsModel& _model;
		EventSubscription<const CollectionItem<BenchmarkResultModel>&> _resultAdded;
		EventSubscription<> _resultsCleared;
	};

} // namespace Elpida
#endif // ELPIDA_APPLICATION_BENCHMARKRESULTSVIEW_HPP
