#ifndef BENCHMARKCONFIGURATIONVIEW_HPP
#define BENCHMARKCONFIGURATIONVIEW_HPP

#include <QWidget>
#include <vector>
#include <unordered_map>

#include "EventSubscription.hpp"
#include "Models/Abstractions/CollectionItem.hpp"
#include "Controllers/BenchmarkConfigurationInstanceController.hpp"

namespace Elpida::Application
{
	namespace Ui
	{
		class CustomBenchmarkConfigurationView;
	}

	class CustomBenchmarkModel;
	class BenchmarkModel;
	class BenchmarkConfigurationModel;
	class ConfigurationView;
	class ConfigurationViewPool;

	class CustomBenchmarkConfigurationView : public QWidget
	{
	 Q_OBJECT
	 public:
		explicit CustomBenchmarkConfigurationView(const CustomBenchmarkModel& benchmarksModel, ConfigurationViewPool& configurationViewPool);
		~CustomBenchmarkConfigurationView() override;
	 private:
		Ui::CustomBenchmarkConfigurationView* _ui;
		const CustomBenchmarkModel& _benchmarksModel;
		ConfigurationViewPool& _configurationViewPool;
		const BenchmarkModel* _currentBenchmark;
		EventSubscription<> _currentBenchmarkChanged;
		std::unordered_map<const BenchmarkConfigurationModel*, ConfigurationView*> _rentedViews;

		void ClearViews();
		void Remove(const BenchmarkConfigurationModel& model);
		void Add(const BenchmarkConfigurationModel& model);
	};
}

#endif // BENCHMARKCONFIGURATIONVIEW_HPP
