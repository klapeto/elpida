#ifndef ELPIDA_APPLICATION_BENCHMARKRESULTSVIEW_HPP
#define ELPIDA_APPLICATION_BENCHMARKRESULTSVIEW_HPP

#include "EventSubscription.hpp"
#include "Models/Abstractions/CollectionItem.hpp"
#include "Models/Benchmark/BenchmarkResultModel.hpp"
#include <QWidget>

namespace Elpida::Application
{

	class CustomBenchmarkResultsModel;

	namespace Ui
	{
		class CustomBenchmarkResultsView;
	}

	class CustomBenchmarkResultsView : public QWidget
	{
	 Q_OBJECT

	 public:
		explicit CustomBenchmarkResultsView(const CustomBenchmarkResultsModel& model, QWidget* parent = nullptr);
		~CustomBenchmarkResultsView() override;

	 private:
		Ui::CustomBenchmarkResultsView* _ui;
		const CustomBenchmarkResultsModel& _model;
		EventSubscription<const CollectionItem<BenchmarkResultModel>&> _resultAdded;
		EventSubscription<> _resultsCleared;
	};

} // namespace Elpida
#endif // ELPIDA_APPLICATION_BENCHMARKRESULTSVIEW_HPP
