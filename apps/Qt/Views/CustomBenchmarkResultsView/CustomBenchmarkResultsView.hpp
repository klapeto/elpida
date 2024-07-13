#ifndef ELPIDA_APPLICATION_BENCHMARKRESULTSVIEW_HPP
#define ELPIDA_APPLICATION_BENCHMARKRESULTSVIEW_HPP

#include "EventSubscription.hpp"
#include "Models/Abstractions/CollectionItem.hpp"
#include "Models/Benchmark/BenchmarkResultModel.hpp"
#include <QWidget>

namespace Elpida::Application
{

	class CustomBenchmarkModel;

	namespace Ui
	{
		class CustomBenchmarkResultsView;
	}

	class CustomBenchmarkResultsView : public QWidget
	{
	 Q_OBJECT

	 public:
		explicit CustomBenchmarkResultsView(const CustomBenchmarkModel& model, QWidget* parent = nullptr);
		~CustomBenchmarkResultsView() override;

	 private:
		Ui::CustomBenchmarkResultsView* _ui;
		const CustomBenchmarkModel& _model;
		EventSubscription<const CollectionItem<BenchmarkResultModel>&> _resultAdded;
		EventSubscription<> _resultsCleared;

		void OnResultsCleared();
	};

} // namespace Elpida
#endif // ELPIDA_APPLICATION_BENCHMARKRESULTSVIEW_HPP
